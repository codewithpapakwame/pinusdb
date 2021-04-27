/*
* Copyright (c) 2019 ChangSha JuSong Soft Inc. <service@pinusdb.cn>.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; version 3 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; If not, see <http://www.gnu.org/licenses>
*/

#include "db/db_impl.h"

#include <algorithm>
#include <vector>
#include <condition_variable>
#include "boost/filesystem.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/foreach.hpp"

#include "pdb_error.h"
#include "server/table_config.h"
#include "db/page_pool.h"
#include "internal.h"

#include "expr/column_item.h"
#include "expr/parse.h"
#include "util/string_tool.h"

#include "util/log_util.h"
#include "util/coding.h"

#include "server/user_config.h"
#include "util/date_time.h"
#include "util/performance_counter.h"
#include "server/event_handle.h"
#include "global_variable.h"
#include "sys_table_schema.h"

#include "util/dbg.h"

DBImpl* DBImpl::dbImpl_ = nullptr;

DBImpl::DBImpl()
{
  pCmtLogTask_ = nullptr;
  pSyncTask_ = nullptr;
  pCompTask_ = nullptr;
  isInit_ = false;
}

DBImpl::~DBImpl()
{
}

DBImpl* DBImpl::GetInstance()
{
  if (dbImpl_ == nullptr)
  {
    dbImpl_ = new DBImpl();
  }
  return dbImpl_;
}

PdbErr_t DBImpl::Start()
{
  PdbErr_t retVal = PdbE_OK;
  //�򿪱�
  const std::vector<TableItem*>& tabVec = pGlbTabCfg->GetTables();
  for (auto tabIt = tabVec.begin(); tabIt != tabVec.end(); tabIt++)
  {
    std::string tabName = (*tabIt)->GetTabName();
    retVal = pGlbTableSet->OpenTable(tabName.c_str());
    if (retVal != PdbE_OK)
    {
      LOG_ERROR("faield to open table({}), err: {}", (*tabIt)->GetTabName().c_str(), retVal);
      return retVal;
    }

    const std::vector<PartItem>& partVec = (*tabIt)->GetPartVec();
    for (auto partIt = partVec.begin(); partIt != partVec.end(); partIt++)
    {
      int32_t partType = partIt->GetPartType();
      int32_t partCode = partIt->GetPartDate();
      std::string partDateStr = partIt->GetPartDateStr();

      if (partType == PDB_PART_TYPE_NORMAL_VAL
        || partType == PDB_PART_TYPE_COMPRESS_VAL)
      {
        retVal = pGlbTableSet->OpenDataPart(tabName.c_str(), partCode, (partType == PDB_PART_TYPE_NORMAL_VAL));
        if (retVal != PdbE_OK)
          return retVal;
      }
      else
      {
        LOG_ERROR("failed to open table({}) datapart({}), not supported file type: {}", 
          tabName.c_str(), partDateStr.c_str(), partType);
        return PdbE_TABLE_CFG_ERROR;
      }
    }

    //�ָ�DoubleWrite����
    retVal = pGlbTableSet->RecoverDW(tabName.c_str());
    if (retVal != PdbE_OK)
    {
      LOG_ERROR("failed to recover table ({}) double write file, err: {}", tabName.c_str(), retVal);
      return retVal;
    }
  }

  //�ָ�������־����
  retVal = RecoverDataLog();
  if (retVal != PdbE_OK)
  {
    return retVal;
  }

  pCmtLogTask_ = new std::thread(&DBImpl::SyncCmtLog, this);
  pSyncTask_ = new std::thread(&DBImpl::SyncTask, this);
  pCompTask_ = new std::thread(&DBImpl::CompressTask, this);

  isInit_ = true;
  return PdbE_OK;
}

void DBImpl::Stop()
{
  stopVariable_.notify_all();
  pGlbPagePool->NotifyAll();
  pCmtLogTask_->join();
  pCompTask_->join();
  pSyncTask_->join();
  //CloseAllTable ��ͬ�����е�����ҳ
  pGlbTableSet->CloseAllTable();
  //ͬ����������ҳ�����ֻ�ǵ������У�ɾ��������־�ļ�
  pGlbCommitLog->Shutdown();
}

void DBImpl::SyncCmtLog()
{
#ifdef _WIN32
  PDB_SEH_BEGIN(false);
  _SyncCmtLog();
  PDB_SEH_END("synccmtlog", return);
#else
  _SyncCmtLog();
#endif
}

void DBImpl::_SyncCmtLog()
{
  while (glbRunning)
  {
    do {
      std::unique_lock<std::mutex> stopLock(stopMutex_);
      stopVariable_.wait_for(stopLock, std::chrono::seconds(5));
    } while (false);

    if (!glbRunning)
      break;

    pGlbCommitLog->SyncLogFile();
  }
}

void DBImpl::SyncTask()
{
#ifdef _WIN32
  PDB_SEH_BEGIN(false);
  _SyncTask();
  PDB_SEH_END("synctask", return);
#else
  _SyncTask();
#endif
}

void DBImpl::_SyncTask()
{
  int64_t dbTick = 0;
  int64_t syncAllTick = 0;
  int32_t writeCacheMB = pGlbSysCfg->GetWriteCache(); // *NORMAL_PAGE_SIZE;

  while (glbRunning)
  {
    do {
      std::unique_lock<std::mutex> stopLock(stopMutex_);
      stopVariable_.wait_for(stopLock, std::chrono::seconds(1));
    } while (false);

    if (!glbRunning)
      break;

    dbTick++;
    if ((dbTick - syncAllTick) >= 1800)
    {
      //�����ϴ�ȫ��ˢ�̳�����Сʱ������ȫ��ˢ��
      LOG_INFO("timeout to sync all page cache to datafile");
      pGlbTableSet->SyncDirtyPages(true);
      LOG_INFO("sync all page cache to datafile finished");
      syncAllTick = dbTick;
    }
    else
    {
      //ÿ���ж��Ƿ��������µ���־�ļ��������µ���־�ļ�����Ҫˢ��
      size_t dirtySizeMB = pGlbTableSet->GetDirtySizeMB();
      if (pGlbCommitLog->NeedSyncAllPage())
      {
        LOG_INFO("create new data log file, sync all page cache to datafile");
        pGlbTableSet->SyncDirtyPages(true);
        LOG_INFO("sync all page cache to datafile finished");
        syncAllTick = dbTick;
      }
      else if (dirtySizeMB >= writeCacheMB)
      {
        //�ж��Ƿ���ҳ̫�࣬��Ҫˢ��
        LOG_INFO("dirty memory ({}MB), sync all page cache to datafile", dirtySizeMB);
        pGlbTableSet->SyncDirtyPages(true);
        LOG_INFO("sync all page cache to datafile finished");
        syncAllTick = dbTick;
      }
      else
      {
        pGlbTableSet->SyncDirtyPages(false);
      }
    }
  }
}

void DBImpl::CompressTask()
{
#ifdef _WIN32
  PDB_SEH_BEGIN(false);
  _CompressTask();
  PDB_SEH_END("compresstask", return);
#else
  _CompressTask();
#endif
}

void DBImpl::_CompressTask()
{
  while (glbRunning)
  {
    do {
      std::unique_lock<std::mutex> stopLock(stopMutex_);
      stopVariable_.wait_for(stopLock, std::chrono::seconds(600));
    } while (false);

    if (!glbRunning)
      break;

    if (pGlbSysCfg->GetCompressFlag())
      pGlbTableSet->DumpToCompress();

    if (glbRunning)
      pGlbTableSet->UnMapCompressData();
  }
}

PdbErr_t DBImpl::RecoverDataLog()
{
  PdbErr_t retVal = PdbE_OK;
  RefUtil tabRef;
  PDBTable* pTab = nullptr;
  uint64_t tabCrc = 0;
  uint64_t tstamp = 0;
  Arena arena;

  std::vector<LogRecInfo> recVec;
  const size_t RedoBufSize = PDB_KB_BYTES(128);
  char* pRedoBuf = arena.AllocateAligned(RedoBufSize);

  while (true)
  {
    recVec.clear();
    pGlbCommitLog->GetRedoRecList(recVec, pRedoBuf, RedoBufSize);
    if (recVec.empty())
      break;

    for (auto rec = recVec.begin(); rec != recVec.end(); rec++)
    {
      if (pTab == nullptr || tabCrc != rec->tabCrc)
      {
        pTab = pGlbTableSet->GetTable(rec->tabCrc, &tabRef);
        if (pTab == nullptr)
        {
          LOG_INFO("failed to recover commit log, table ({}) not found");
          continue;
        }
        tabCrc = rec->tabCrc;
      }

      tstamp = Coding::FixedDecode64((rec->pRec + 2));
      retVal = pTab->InsertByDataLog(rec->metaCrc, rec->devId, tstamp, rec->pRec, rec->recLen);
      if (retVal != PdbE_OK && retVal != PdbE_TABLE_FIELD_MISMATCH)
      {
        LOG_ERROR("failed to recover commit log, table ({}), devid ({}), tstamp:({}), retVal:({})",
          rec->tabCrc, rec->devId, tstamp, retVal);
      }
    }
  }

  return PdbE_OK;
}
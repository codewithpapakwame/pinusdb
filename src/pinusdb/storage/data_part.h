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

#pragma once
#include "internal.h"
#include "port/os_file.h"
#include "util/arena.h"
#include "util/ker_list.h"
#include "query/iquery.h"
#include "query/data_part_query_param.h"
#include "storage/normal_part_idx.h"
#include "storage/normal_data_page.h"
#include "util/ref_util.h"
#include <atomic>

typedef struct _DataFileMeta
{
  char headStr_[16];            
  char pageSize_[4]; //uint32_t
  char fieldCnt_[4]; //uint32_t
  char partCode_[4]; //uint32_t
  char tableType_[4]; //uint32_t

  FieldInfoFormat fieldRec_[PDB_TABLE_MAX_FIELD_COUNT];
  char padding_[10460]; 
  char crc_[4]; //uint32_t
}DataFileMeta;

class DataPart : public RefObj
{
public:
  DataPart()
  {
    refCnt_ = 0;
    bgDayTs_ = 0;
    edDayTs_ = 0;
  }

  virtual ~DataPart() { }

  virtual void Close() = 0;
  virtual PdbErr_t RecoverDW(const char* pPageBuf) 
  {
    return PdbE_FILE_READONLY;
  }

  virtual PdbErr_t InsertRec(uint32_t metaCode, int64_t devId, int64_t tstamp,
    bool replace, const char* pRec, size_t recLen)
  {
    return PdbE_FILE_READONLY;
  }

  PdbErr_t QueryAsc(const std::list<int64_t>& devIdList,
    const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut);
  PdbErr_t QueryDesc(const std::list<int64_t>& devIdList,
    const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut);
  PdbErr_t QueryFirst(std::list<int64_t>& devIdList,
    const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut);
  PdbErr_t QueryLast(std::list<int64_t>& devIdList,
    const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut);
  PdbErr_t QuerySnapshot(std::list<int64_t>& devIdList,
    const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut);

  virtual PdbErr_t UnMap() { return PdbE_OK; }

  virtual PdbErr_t DumpToCompPart(const char* pDataPath) { return PdbE_OK; }
  virtual bool SwitchToReadOnly() { return true; }
  virtual bool IsPartReadOnly() const { return true; }
  virtual bool IsNormalPart() const { return false; }
  virtual uint32_t GetMetaCode() const { return 0; }
  virtual PdbErr_t SyncDirtyPages(bool syncAll, OSFile* pDwFile) { return PdbE_FILE_READONLY; }
  virtual PdbErr_t AbandonDirtyPages() { return PdbE_OK; }
  virtual size_t GetDirtyPageCnt() { return 0; }

  uint32_t GetPartCode() const { return static_cast<uint32_t>(bgDayTs_ / DateTime::MicrosecondPerDay); }
  std::string GetIdxPath() const { return idxPath_; }
  std::string GetDataPath() const { return dataPath_; }

protected:
  virtual PdbErr_t QueryDevAsc(int64_t devId, const DataPartQueryParam& queryParam,
    IQuery* pQuery, uint64_t timeOut, bool queryFirst, bool* pIsAdd) = 0;
  virtual PdbErr_t QueryDevDesc(int64_t devId, const DataPartQueryParam& queryParam,
    IQuery* pQuery, uint64_t timeOut, bool queryLast, bool* pIsAdd) = 0;
  virtual PdbErr_t QueryDevSnapshot(int64_t devId, const DataPartQueryParam& queryParam,
    IQuery* pQuery, uint64_t timeOut, bool* pIsAdd) = 0;

  PdbErr_t InitQueryParam(DataPartQueryParam& queryParam, const TableInfo* pTabInfo, IQuery* pQuery);
  virtual const std::vector<FieldInfo>& GetFieldInfoVec() const = 0;
  virtual const std::vector<size_t>& GetFieldPosVec() const = 0;
  virtual bool SupportPreWhere() const { return false; }

protected:
  std::string idxPath_;
  std::string dataPath_;

  int64_t bgDayTs_;
  int64_t edDayTs_;
};


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

#include <unordered_map>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>

#include "pdb.h"
#include "table/db_obj.h"
#include "table/table_info.h"
#include "expr/sql_parser.h"
#include "query/data_table.h"
#include "query/result_filter.h"
#include "table/table_info.h"
#include "query/result_filter_raw.h"

class DBImpl
{
private:
  DBImpl();

public:
  virtual ~DBImpl();

  static DBImpl* GetInstance();
  
  PdbErr_t Start();
  void Stop();

  void SyncTask();
  void CompressTask();

private:
  void _SyncTask();
  void _CompressTask();

private:
  PdbErr_t RecoverDataLog();

private:
  std::thread* pSyncTask_;  //����ͬ���������ļ�
  std::thread* pRepTask_;   //����ͬ�����Զ����ݿ�
  std::thread* pCompTask_;  //��ʷ����ѹ��
  bool isInit_;

  std::mutex stopMutex_;
  std::condition_variable stopVariable_;

  static DBImpl* dbImpl_;
};


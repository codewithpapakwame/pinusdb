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
#include "util/ker_list.h"
#include "query/result_filter.h"

typedef struct _ConnInfo
{
  int role_;
  int remotePort_;
  int64_t connTime_;
  char remoteHost_[IP_ADDR_STR_LEN];
  char loginName_[PDB_USER_NAME_LEN];
}ConnInfo;

class QueryParam;
class DataTable;

class ServerConnection
{
public:
  ServerConnection();
  ~ServerConnection();

  bool AddConnection(uint64_t connKey, const char* pRemoteHost, int remotePort);
  void ConnectionLogin(uint64_t connKey, const char* pLoginName, int role);

  void DelConnection(uint64_t connKey);

  PdbErr_t QueryConn(IResultFilter* pFilter);

private:
  enum {
    kConnCntPerBlk = 64,  //һ���ڴ����ConnInfo����ĸ���, ���� 8K
    kMaxBlkCnt = 16,     //�ڴ�����������Ҳ��������ܴ洢 256 * 16 = 4096 �����Ӷ���
  };

  ConnInfo* MallocConnInfo();

private:
  std::mutex connMutex_;
  std::unordered_map<uint64_t, ConnInfo*> connMap_;

  struct list_head freeConnList_;

  int blkCnt_;                //�ڴ������
  void* ppBlk_[kMaxBlkCnt];   //�ڴ������

};



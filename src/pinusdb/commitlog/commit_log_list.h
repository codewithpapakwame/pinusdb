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
#include "commitlog/commit_log_file.h"

class CommitLogList
{
public:
  CommitLogList();
  ~CommitLogList();

  PdbErr_t Init(const char* pLogPath);
  PdbErr_t AppendRec(uint64_t tabCrc, uint32_t metaCrc, int recType,
    int64_t devId, const char* pRec, size_t recLen);

  PdbErr_t GetRedoRecList(std::vector<LogRecInfo>& recList, char* pRedoBuf, size_t bufSize);
  bool NeedSyncAllPage();

  void MakeSyncPoint();
  void CommitSyncPoint();
  void Shutdown();

  void SyncLogFile();

  int64_t GetLogPosition() const;

private:
  PdbErr_t NewLogFile();
  PdbErr_t InitLogList();
  PdbErr_t AppendPoint();
  void RemoveExpiredLog();

private:
  std::mutex logMutex_;
  uint32_t nextFileCode_;      // ��һ���ļ����

  uint64_t curSyncPos_;         // ͬ��-λ��-���ڽ��ڴ�������д�����
  uint64_t nextSyncPos_;

  CommitLogFile* pWriteLog_;     // д��־�ļ�

  std::string logPath_;        // ��־·��
  std::mutex vecMutex_;
  std::vector<CommitLogFile*> logFileVec_;  //��־�ļ��б�
};





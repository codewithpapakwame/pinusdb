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

#include "port/os_file.h"
#include "commitlog/commit_log_block.h"

#define BLKTYPE_FILE_HEAD      1  //�ļ�ͷ��Ϣ
#define BLKTYPE_SYNC_INFO      2
#define BLKTYPE_INSERT_CLI     3  // ���ݲ����ɿͻ��˷�����Ҫͬ����ͬ������ݿ�
#define BLKTYPE_INSERT_REP     4  // ���ݲ�����ͬ�����ݿⷢ�𣬲���Ҫͬ����ͬ������ݿ�

#define DATA_LOG_FILE_VER      2
#define DATA_LOG_FILE_SIZE     (512 * 1024 * 1024)  //ÿ����־�ļ����512M

typedef struct _BlkHdr
{
  int32_t  blkType_;         // ������
  uint32_t hdrCrc_;          // ͷ��У��
}BlkHdr;

typedef struct _LogFileHdr {
  uint32_t logFileVer_;      // �ļ��汾
  uint32_t logFileCode_;     // �ļ����
  char padding_[16];
  BlkHdr blkHdr_;
}LogFileHdr;

typedef struct _LogBlkHdr {
  uint64_t tabCrc_;          // ��
  uint32_t metaCode_;         // �ֶ�У��
  int32_t  recCnt_;          // ��¼����
  int32_t  dataLen_;         // ���ݳ���
  uint32_t dataCrc_;         // ����У��
  BlkHdr   blkHdr_;          // ��ͷ����Ϣ
}LogBlkHdr;

typedef struct _SyncBlkHdr {
  uint64_t repPos_;      // ���ļ�λ��
  uint64_t syncPos_;     // ����ͬ��λ��
  uint64_t padding_;
  BlkHdr   blkHdr_;      // ��ͷ����Ϣ
}SyncBlkHdr;

class CommitLogFile
{
public:
  CommitLogFile();
  ~CommitLogFile();

  PdbErr_t OpenLog(uint32_t fileCode, const char* pPath);
  PdbErr_t NewLog(uint32_t fileCode, const char* pPath, uint64_t grpPos, uint64_t syncPos);
  PdbErr_t Close();
  void Sync();
  uint32_t GetFileCode() const { return fileCode_; }
  uint64_t GetCurPos() const { return curPos_; }
  std::string GetFilePath() const { return filePath_; }

  PdbErr_t AppendData(const LogBlkHdr* pLogHdr, const CommitLogBlock* pLogBlock);
  PdbErr_t AppendSync(uint64_t repPos, uint64_t syncPos);
  PdbErr_t ReadBuf(uint8_t* pBuf, int readLen, int64_t offset);
  PdbErr_t RecoverPoint(uint64_t* pRepPos, uint64_t* pSyncPos);

private:
  std::string filePath_;
  OSFile logFile_;
  uint64_t curPos_;
  uint64_t syncLen_;
  uint64_t syncTime_;
  uint32_t fileCode_;
};

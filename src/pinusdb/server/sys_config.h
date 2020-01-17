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
#include "query/result_filter.h"

class SysConfig
{
public:
  SysConfig();
  ~SysConfig();

  std::string GetAddress() const { return address_; }
  int32_t GetPort() const { return port_; }
  int32_t GetLogLevel() const { return logLevel_; }
  int32_t GetQueryTimeOut() const { return queryTimeOut_; }
  int32_t GetCacheSize() const { return cacheSize_; }
  void UpdateCacheSize(int32_t cacheSize) { cacheSize_ = cacheSize; }
  int32_t GetInsertValidDay() const { return insertValidDay_; }
  std::string GetSysLogPath() const { return sysLogPath_; }
  std::string GetNormalDataPath() const { return normalDataPath_; }
  std::string GetCompressDataPath() const { return compressDataPath_; }
  std::string GetTablePath() const { return tabPath_; }
  std::string GetCommitLogPath() const { return commitLogPath_; }
  int32_t GetDevCnt() const { return PDB_SYS_DEV_CNT; }
  bool GetCompressFlag() const { return compressFlag_; }

  bool LoadConfig();

  PdbErr_t Query(IResultFilter* pFilter);

private:
  std::mutex sysCfgMutex_;
  std::string address_;          // ���������ַ
  int32_t port_;                 // ����˿�
  int32_t logLevel_;             // ��־����
  int32_t queryTimeOut_;         // ��ѯ��ʱʱ��
  int32_t cacheSize_;            // �����С
  int32_t insertValidDay_;       // ������Чʱ��
  std::string tabPath_;          // ��Ŀ¼
  std::string normalDataPath_;   // ��������Ŀ¼
  std::string compressDataPath_; // ѹ������Ŀ¼
  std::string commitLogPath_;    // ������־Ŀ¼
  std::string sysLogPath_;       // ϵͳ��־Ŀ¼
  bool compressFlag_;            // �Ƿ�����ѹ��
};
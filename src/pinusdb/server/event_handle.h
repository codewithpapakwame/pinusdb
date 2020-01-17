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

#include <string>
#include <mutex>
#include "pdb.h"
#include "server/proto_header.h"
#include "query/data_table.h"
#include "expr/sql_parser.h"
#include "util/arena.h"
#include "expr/insert_sql.h"

class EventHandle
{
public:
  EventHandle(int socket, const char* pRemoteIp, int remotePort);
  ~EventHandle();

#ifdef _WIN32
  bool RecvPostedEvent(const uint8_t* pBuf, size_t bytesTransfered);
  bool SendPostedEvent(size_t bytesTransfered);
  bool GetRecvBuf(WSABUF* pWsaBuf);
  bool GetSendBuf(WSABUF* pWsaBuf);
#else
  bool RecvData();
  bool SendData();

  int GetSocket() const { return socket_; }
  inline void AddRef() { this->refCnt_.fetch_add(1); }
  inline bool MinusRef() { return this->refCnt_.fetch_sub(1) == 1; }
protected:
  bool _SendData();
#endif

public:
  bool ExecTask();

  inline int GetState() { return eventState_; }
  inline bool IsEnd() { return eventState_ == EventState::kEnd; }
  inline void SetEnd() { eventState_ = EventState::kEnd; }

  enum EventState
  {
    kRecv = 1,     //���ڽ��ձ���
    kExec = 2,     //����ִ�л�ȴ���ִ��
    kSend = 3,  //���ڷ��ͷ��ر���
    kEnd = 4,   //ȫ���������ύ
  };
protected:

  PdbErr_t DecodeHead();

  PdbErr_t DecodeSqlPacket(const char** ppSql, size_t* pSqlLen);
  PdbErr_t DecodeInsertTable(InsertSql* pInsertSql);
  PdbErr_t DecodeInsertSql(InsertSql* pInsertSql, Arena* pArena);

  PdbErr_t EncodeQueryPacket(PdbErr_t retVal, DataTable* pTable);
  PdbErr_t EncodeInsertPacket(PdbErr_t retVal, int32_t successCnt);
  PdbErr_t EncodeInsertTablePacket(PdbErr_t retVal, const std::list<PdbErr_t>& insertRet);

  PdbErr_t ExecLogin();

  PdbErr_t ExecQuery(DataTable* pResultTable);
  PdbErr_t ExecInsertSql(int32_t* pSuccessCnt);
  PdbErr_t ExecInsertTable(std::list<PdbErr_t>& resultList);
  PdbErr_t ExecNonQuery();

  PdbErr_t _DropTable(const DropTableParam* pDropTableParam);
  PdbErr_t _DeleteDev(const DeleteParam* pDeleteParam);
  PdbErr_t _AttachTable(const AttachTableParam* pAttachTableParam);
  PdbErr_t _DetachTable(const DetachTableParam* pDetachTableParam);
  PdbErr_t _AttachFile(const AttachFileParam* pAttachFileParam);
  PdbErr_t _DetachFile(const DetachFileParam* pDetachFileParam);
  PdbErr_t _DropDataFile(const DropFileParam* pDropFileParam);
  PdbErr_t _CreateTable(const CreateTableParam* pCreateTableParam);
  PdbErr_t _AddUser(const AddUserParam* pAddUserParam);
  PdbErr_t _ChangePwd(const ChangePwdParam* pChangePwdParam);
  PdbErr_t _ChangeRole(const ChangeRoleParam* pChangeRoleParam);
  PdbErr_t _DropUser(const DropUserParam* pDropUserParam);
  PdbErr_t _AlterTable(const CreateTableParam* pCreateTableParam);

  PdbErr_t AllocRecvBuf(size_t bufLen);
  void FreeRecvBuf();

  PdbErr_t AllocSendBuf(size_t bufLen);
  void FreeSendBuf();

protected:
#ifndef _WIN32
  std::mutex eventMutex_;
  std::atomic<int> refCnt_;
  int socket_;
#endif

  int32_t eventState_;
  int32_t remotePort_;
  int32_t userRole_;                   //��¼�Ľ�ɫ
  std::string remoteIp_;
  std::string userName_;   //��¼���û�

  uint32_t packetVersion_;                 //���ĵİ汾
  uint32_t method_;                        //ִ�еķ���
  uint32_t fieldCnt_;                      //�ֶ�����
  uint32_t recCnt_;                        //��¼����
  uint32_t dataCrc_;                       //���ݵ�CRC

  size_t   recvHeadLen_;                    //���յ��ı���ͷ����
  uint8_t  recvHead_[ProtoHeader::kProtoHeadLength];   //����ͷ����
  size_t   recvBodyLen_;                    //���յ��ı����峤��
  size_t   totalRecvBodyLen_;               //��������ܳ���

  uint8_t* pRecvBuf_;                      //���ջ���
  size_t   recvBufLen_;                    //���ջ����С

  size_t   sendLen_;                        //�ѷ��ͱ��ĳ���
  size_t   totalSendLen_;                   //���ͱ����ܳ���

  size_t   sendBufLen_;                     //���ͻ����С
  uint8_t* pSendBuf_;                      //���ͻ���

};




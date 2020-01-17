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
#include "util/object_pool.h"
#include <thread>
#include <atomic>
#include <condition_variable>

class EventHandle;

#ifdef _WIN32

#define SOCKET_CONTEXT_BUF_SIZE   (8 * 1024)

typedef enum _OPERATION_TYPE
{
  ACCEPT_POSTED = 0,      // Ͷ��Accept����
  RECV_POSTED = 1,        // Ͷ�ݽ��ղ���
  SEND_POSTED = 2,        // Ͷ�ݷ��Ͳ���
  NULL_POSTED = 3,        // ���ڳ�ʼ��
}OPERATION_TYPE;

typedef struct _SOCKET_CONTEXT
{
  SOCKET         socket_;         //�׽���
  OVERLAPPED     overlapped_;     //�ص���������
  OPERATION_TYPE opType_;         //��������
  WSABUF         wsaBuf_;         //�������
  EventHandle*   pEventHandle_;   //�¼�����
  char           dataBuf_[SOCKET_CONTEXT_BUF_SIZE]; //���ջ�����
}SOCKET_CONTEXT;

#endif

class ServerPDB
{
public:
  ServerPDB();
  ~ServerPDB();

public:
  bool Start();
  void Stop();

#ifdef _WIN32

public:
  bool LoadSocketLib();
  void UnloadSocketLib();

private:
  bool InitIOCP();
  bool InitListenSocket();
  SOCKET_CONTEXT* DoAccept(SOCKET_CONTEXT* pContext, size_t bytesTransfered);
  SOCKET_CONTEXT* DoFirstRecvWithData(SOCKET_CONTEXT* pContext, size_t bytesTransfered);

  SOCKET_CONTEXT* DoFirstRecvWithoutData(SOCKET_CONTEXT* pContext);

  bool PostAccept(SOCKET_CONTEXT* pContext);

  bool PostRecv(SOCKET_CONTEXT* pContext);
  bool PostSend(SOCKET_CONTEXT* pContext);

  bool StartIocp(const char* pSerIp, int serPort);
  void StopIocp();

  int GetProcessorsCnt();

  SOCKET_CONTEXT* MallocSocketContext();
  void FreeSocketContext(SOCKET_CONTEXT* pContext);

  static DWORD WINAPI WorkerThread(LPVOID lpParam);
  static DWORD WorkerMain(LPVOID lpParam);

#else

public:
  bool StartEpoll(const char* pSerIp, int serPort);
  void StopEpoll();

  void EpollThread();
  void WorkerThread();

#endif

private:
  enum {
    kMaxWorkThreadCnt = 128,    //�����߳���
    kThreadsPerProcessor = 8,   //ÿһ�������������ж��ٸ��߳�
    kConcurrentPerProcessor = 4, //ÿһ����������ͬʱ������߳���

    kSocketListenCnt = 1,      //�����׽��ָ���
    kSocketAcceptCnt = 15,     //�ύ��Accept����
    kSocketWorkerCnt = 1024,   //�׽������Ӹ���

    kContextCntPerBlk = 16,     //һ���ڴ����64������
  };

  EventHandle* NewEvent(int socket, const char* pRemoteIp, int remotePort);
  void RemoveEvent(EventHandle* pEvent);

  bool InitLog();
  bool InitUser();
  bool InitTable();
  bool InitCommitLog();

private:
  std::string serIP_;
  int         serPort_;

#ifdef _WIN32

  HANDLE iocpHandle_;

  SOCKET_CONTEXT* pListenContext_;

  LPFN_ACCEPTEX              lpfnAcceptEx_;
  LPFN_GETACCEPTEXSOCKADDRS  lpfnGetAcceptExSockAddrs_;

  int workThreadCnt_;
  HANDLE workThreads_[kMaxWorkThreadCnt];

  std::mutex iocpMutex_;
  ObjectPool* pContextPool_;
#else

  int epollFd_;
  int licenseFd_;
  std::thread* workThreads_[kMaxWorkThreadCnt];

  std::mutex taskMutex_;
  std::list<EventHandle*> taskList_;
  std::condition_variable taskVariable_;

#endif

};


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

#ifdef _WIN32

#include "util/dbg.h"
#include <DbgHelp.h>
#include <time.h>
#include "global_variable.h"

int crash_handler(char const* module, struct _EXCEPTION_POINTERS *ep, DWORD thread_id/* = 0*/)
{
  time_t tick = 0;
  time(&tick);
  tm _now = { 0 };
  localtime_s(&_now, &tick);

  std::string logPath = pGlbSysCfg->GetSysLogPath();

  unsigned code = ep ? ep->ExceptionRecord->ExceptionCode : 0;
  char dmpFile[MAX_PATH] = { 0 };
  sprintf_s(dmpFile,
    "%s/PDB%04d%02d%02d%02d%02d%02d0x%08x.dmp",
    logPath.c_str(),
    _now.tm_year + 1900,
    _now.tm_mon + 1,
    _now.tm_mday,
    _now.tm_hour,
    _now.tm_min,
    _now.tm_sec,
    code);
  
  HANDLE hFile = CreateFile(dmpFile,
    GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ | FILE_SHARE_WRITE,
    NULL,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    return EXCEPTION_CONTINUE_SEARCH;
  }

  MINIDUMP_EXCEPTION_INFORMATION ex = { 0 };
  ex.ExceptionPointers = ep;
  ex.ClientPointers = TRUE;
  ex.ThreadId = thread_id == 0 ? GetCurrentThreadId() : thread_id;

  int flag = MiniDumpNormal |
    MiniDumpWithDataSegs |
    MiniDumpWithHandleData |
    MiniDumpWithIndirectlyReferencedMemory;

  ::MiniDumpWriteDump(
    ::GetCurrentProcess()
    , GetCurrentProcessId()
    , hFile
    , (MINIDUMP_TYPE)flag
    , ep ? &ex : 0
    , 0
    , 0);

  CloseHandle(hFile);

  return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI exception_handler(_EXCEPTION_POINTERS *ExceptionInfo)
{
  //printf("using exception_handler\n");
  crash_handler_with_stack_overflow("GLOBAL", ExceptionInfo);
  return EXCEPTION_EXECUTE_HANDLER;
}

void purcall_handler()
{
  printf("using purcall_handler.\n");
  crash_handler_with_stack_overflow("purcall", 0);
}
void invalid_param_handler(const wchar_t* /*expression*/
  , const wchar_t* /*function*/
  , const wchar_t* /*file*/
  , unsigned int /*line*/
  , uintptr_t /*pReserved*/)
{
  printf("using invalid_param_handler.\n");
  crash_handler_with_stack_overflow("CRT", 0);
}

int crash_handler_with_stack_overflow(char const* module, struct _EXCEPTION_POINTERS *ep)
{
  unsigned code = ep ? ep->ExceptionRecord->ExceptionCode : 0;

  if (code == EXCEPTION_STACK_OVERFLOW /*0xc00000fd*/)
  {
    //ջ�����Ҫ���µ��߳��д���dump�ļ������򴴽�dump�ļ��ĺ����ֻ����һ��ջ����쳣���Ӷ����·����˳�
    int ret_val = 0;
    //DWORD thread_id = GetCurrentThreadId();
    //thread t([&]()
    //{
    //  ret_val = crash_handler(module, ep, thread_id);
    //});
    //t.join();
    return ret_val;
  }
  else
  {
    return crash_handler(module, ep);
  }
}

#endif
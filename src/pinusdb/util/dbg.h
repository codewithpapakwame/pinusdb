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

#ifdef _WIN32

#include "internal.h"

int crash_handler(char const* module, struct _EXCEPTION_POINTERS *ep, DWORD thread_id = 0);

// ����� crash handler, �����˶�ջ�����������⴦�������뿴����ʵ�ִ���
int crash_handler_with_stack_overflow(char const* module, struct _EXCEPTION_POINTERS *ep);

// a global exception handler is needed for 
// threads that not use SEH.
LONG WINAPI exception_handler(_EXCEPTION_POINTERS *ExceptionInfo);
//���麯�������쳣�Ĵ���
void purcall_handler();
// special handler to catch CRT invalid parameter errors, 
// they can not be catched by exception_handler.
void invalid_param_handler(const wchar_t* expression
  , const wchar_t* function
  , const wchar_t* file
  , unsigned int line
  , uintptr_t pReserved);


#define PDB_SEH_BEGIN(main) \
  if (main) { \
    SetUnhandledExceptionFilter(exception_handler); \
    _set_purecall_handler(purcall_handler);  \
    _set_invalid_parameter_handler(invalid_param_handler);  \
  } __try{ do{} while(0)

#define PDB_SEH_END(module, action) \
  }__except(crash_handler_with_stack_overflow(module, GetExceptionInformation())) { \
  if (GetExceptionCode() == EXCEPTION_STACK_OVERFLOW) {} action; } \
  do{} while(0)

#endif

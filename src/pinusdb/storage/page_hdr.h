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
#include "util/ker_list.h"

typedef struct _PageHdr PageHdr;

struct _PageHdr
{
  struct list_head listHdr_; // LRU����
  uint64_t  pageCode_;       // ҳ��  ��ID + ����� + ҳ��, 1+3+4
  uint8_t*  pData_;          // ҳ����

  uint32_t  curVer_;         // ��ǰ�汾��
  uint32_t  syncVer_;        // ��ͬ���İ汾��
  uint32_t  rrVer_;          // ��ת�汾��
  volatile int32_t   nRef_;           // ���ü���
  int32_t  splitGrp_;        // ������

  uint8_t  isInit_;          // �Ƿ񱻳�ʼ��
  uint8_t  isDirty_;         // �Ƿ�����ҳ
  uint8_t  onlyMem_;         // �����ڴ��У�����ʱ���ô���д�̺�Ҫд����
  uint8_t  isFull_;
};

#define PAGEHDR_INIT(pPage, pData)  do {\
  INIT_NODE(&((pPage)->listHdr_)); \
  (pPage)->pageCode_ = 0; \
  (pPage)->pData_ = pData; \
  (pPage)->curVer_ = 0; \
  (pPage)->syncVer_ = 0; \
  (pPage)->rrVer_ = 0; \
  (pPage)->nRef_ = 0; \
  (pPage)->splitGrp_ = 0; \
  (pPage)->isInit_ = 0; \
  (pPage)->isDirty_ = 0; \
  (pPage)->onlyMem_ = 0; \
  (pPage)->isFull_ = 0; \
}while(false)

#define PAGEHDR_CLEAR(pPage)  do { \
  (pPage)->pageCode_ = 0; \
  (pPage)->curVer_ = 0; \
  (pPage)->syncVer_ = 0; \
  (pPage)->rrVer_ = 0; \
  (pPage)->nRef_ = 0; \
  (pPage)->splitGrp_ = 0; \
  (pPage)->isInit_ = 0; \
  (pPage)->isDirty_ = 0; \
  (pPage)->onlyMem_ = 0; \
  (pPage)->isFull_ = 0; \
} while(false)

#define PAGEHDR_SET_PAGECODE(pPage, pageCode) do { \
  (pPage)->pageCode_ = pageCode; \
} while(false)

#define PAGEHDR_IS_DIRTY(pPage)     ((pPage)->isDirty_ != 0)
#define PAGEHDR_DATA_UPDATE(pPage)  do { \
  (pPage)->curVer_++; \
  (pPage)->isDirty_ = 1; \
} while(false)

#define PAGEHDR_ONLY_MEM(pPage)        ((pPage)->onlyMem_ != 0)
#define PAGEHDR_SET_ONLY_MEM(pPage)    ((pPage)->onlyMem_ = 1)

#define PAGEHDR_GET_PAGENO(pPage)      static_cast<int32_t>((pPage)->pageCode_ & 0xFFFFFFFF)
#define PAGEHDR_GET_PAGECODE(pPage)    ((pPage)->pageCode_)

#define PAGEHDR_IS_INIT(pPage)         ((pPage)->isInit_ != 0)
#define PAGEHDR_SET_INIT_TRUE(pPage)   ((pPage)->isInit_ = 1)
#define PAGEHDR_SET_INIT_FALSE(pPage)  ((pPage)->isInit_ = 0)

#define PAGEHDR_ROUND_ROBIN(pPage)     ((pPage)->rrVer_ = (pPage)->curVer_)
#define PAGEHDR_NEED_SYNC(pPage)       ((pPage)->syncVer_ < (pPage)->rrVer_)
#define PAGEHDR_SYNC_DATA(pPage)       do {    \
  (pPage)->syncVer_ = (pPage)->rrVer_;         \
  (pPage)->onlyMem_ = 0;                       \
  (pPage)->isDirty_ = (pPage)->curVer_ == (pPage)->syncVer_ ? 0 : 1;  \
  (pPage)->isFull_ = 0;   \
  (pPage)->splitGrp_ = 0; \
} while(false)

#define PAGEHDR_ADD_REF(pPage)         ((pPage)->nRef_++)
#define PAGEHDR_GET_REF(pPage)         ((pPage)->nRef_)
#define PAGEHDR_CLEAR_REF(pPage)       ((pPage)->nRef_ = 0)
#define PAGEHDR_MINUS_REF(pPage)       do { if ((pPage)->nRef_ > 0) (pPage)->nRef_--; } while(false)

#define PAGEHDR_GET_PAGEDATA(pPage)    ((pPage)->pData_)

#define PAGEHDR_GET_SPLIT_GRP(pPage)      ((pPage)->splitGrp_)
#define PAGEHDR_SET_SPLIT_GRP(pPage, grp) ((pPage)->splitGrp_ = grp)

#define PAGEHDR_GET_IS_FULL(pPage)        ((pPage)->isFull_ == 1)
#define PAGEHDR_SET_IS_FULL(pPage)        ((pPage)->isFull_ = 1)


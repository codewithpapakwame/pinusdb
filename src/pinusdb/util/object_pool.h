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

#include <stdint.h>
#include "util/ker_list.h"

class ObjectPool
{
public:
  ObjectPool(int objSize, int maxCnt, int cntPerBlk);

  virtual ~ObjectPool();

  uint8_t* MallocObject();

  void FreeObject(uint8_t* pObj);

private:
  struct list_head freeList_;     // ��������

  int objSize_;                   // �����С
  int cntPerBlk_;                 // ÿ����洢�Ķ�����

  int maxBlkCnt_;                 // ��������
  int curBlkCnt_;                 // ��ǰ������
  uint8_t** ppBlk_;                  // ���ݿ�ָ��
};


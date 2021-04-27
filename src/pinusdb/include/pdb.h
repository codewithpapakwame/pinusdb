/*
* Copyright (c) 2020 ChangSha JuSong Soft Inc. <service@pinusdb.cn>.
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

typedef int PdbErr_t;

#define PDB_TABLE_NAME_LEN    48  //���������
#define PDB_FILED_NAME_LEN    48  //�ֶ�������
#define PDB_USER_NAME_LEN     48  //�û��������

#define PDB_DEVID_NAME_LEN       96  // �豸������
#define PDB_DEVID_EXPAND_LEN     128 // ��չ���Գ���

#define PDB_MIN_REAL_VALUE       (-1000000000)
#define PDB_MAX_REAL_VALUE       (+1000000000)

enum PDB_VALUE_TYPE
{
  VAL_NULL            = 0,
  VAL_BOOL            = 1,     // bool
  VAL_INT8            = 2,     // 1�ֽ�����
  VAL_INT16           = 3,     // 2�ֽ�����
  VAL_INT32           = 4,     // 4�ֽ�����
  VAL_INT64           = 5,     // 8�ֽ�����
  VAL_DATETIME        = 6,     // ʱ��� 8�ֽ�
  VAL_FLOAT           = 7,     // 4�ֽ� �����ȸ�����
  VAL_DOUBLE          = 8,     // 8�ֽ� ˫���ȸ�����
  VAL_STRING          = 9,     // �ַ���
  VAL_BLOB            = 10,    // ������
};

enum PDB_FIELD_TYPE
{
  TYPE_BOOL           = 1,  // bool
  TYPE_INT8           = 2,  // 1�ֽ�����
  TYPE_INT16          = 3,  // 2�ֽ�����
  TYPE_INT32          = 4,  // 4�ֽ�����
  TYPE_INT64          = 5,  // 8�ֽ�����
  TYPE_DATETIME       = 6,  // ʱ��� 8�ֽ�
  TYPE_FLOAT          = 7,  // 4�ֽ� �����ȸ�����
  TYPE_DOUBLE         = 8,  // 8�ֽ� ˫���ȸ�����
  TYPE_STRING         = 9,  // �ַ���
  TYPE_BLOB           = 10, // ������

  TYPE_REAL2          = 32, // double, ȡֵ��Χ [-999,999,999.99      ~  +999,999,999.99]
  TYPE_REAL3          = 33, // double, ȡֵ��Χ [-999,999,999.999     ~  +999,999,999.999]
  TYPE_REAL4          = 34, // double, ȡֵ��Χ [-999,999,999.9999    ~  +999,999,999.9999]
  TYPE_REAL6          = 35  // double, ȡֵ��Χ [-999,999,999.999999  ~  +999,999,999.999999]
};

#define PDB_TYPE_IS_VALID(type)    (((type) >= PDB_FIELD_TYPE::TYPE_BOOL && (type) <= PDB_FIELD_TYPE::TYPE_BLOB) || ((type) >= PDB_FIELD_TYPE::TYPE_REAL2  && (type) <= PDB_FIELD_TYPE::TYPE_REAL6))
#define PDB_TYPE_IS_REAL(type)     ((type) >= PDB_FIELD_TYPE::TYPE_REAL2 && (type) <= PDB_FIELD_TYPE::TYPE_REAL6)
#define PDB_TYPE_IS_NUMBER(type)   ((type) == PDB_FIELD_TYPE::TYPE_INT8 || (type) == PDB_FIELD_TYPE::TYPE_INT16 || (type) == PDB_FIELD_TYPE::TYPE_INT32 || (type) == PDB_FIELD_TYPE::TYPE_INT64)
#define PDB_TYPE_IS_FLOAT_OR_DOUBLE(type) ((type) == PDB_FIELD_TYPE::TYPE_FLOAT || (type) == PDB_FIELD_TYPE::TYPE_DOUBLE)


enum PDB_ROLE
{
  ROLE_READ_ONLY = 1,    //ֻ��
  ROLE_WRITE_ONLY = 2,   //ֻд
  ROLE_READ_WRITE = 3,   //��д
  ROLE_ADMIN = 4,        //����Ա
};

typedef struct _ColumnInfo
{
  char colName_[PDB_FILED_NAME_LEN];
  int colType_;
}ColumnInfo;

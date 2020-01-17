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
  VAL_INT64           = 2,     // 8�ֽ� bigint
  VAL_DATETIME        = 3,    // ʱ��� 8�ֽ�
  VAL_DOUBLE          = 4,     // 8�ֽ� ˫���ȸ�����
  VAL_STRING          = 5,     // �ַ���
  VAL_BLOB            = 6,     // ������
};

enum PDB_FIELD_TYPE
{
  TYPE_BOOL             = 1,     // bool
  TYPE_INT64            = 2,     // 8�ֽ�
  TYPE_DATETIME         = 3,     // 8�ֽ�
  TYPE_DOUBLE           = 4,     // 8�ֽ� ˫���ȸ�����
  TYPE_STRING           = 5,     // �ַ���
  TYPE_BLOB             = 6,     // ������

  TYPE_REAL2            = 32,     // double, ȡֵ��Χ [-999,999,999.99      ~  +999,999,999.99]
  TYPE_REAL3            = 33,     // double, ȡֵ��Χ [-999,999,999.999     ~  +999,999,999.999]
  TYPE_REAL4            = 34,    // double, ȡֵ��Χ [-999,999,999.9999    ~  +999,999,999.9999]
  TYPE_REAL6            = 35,    // double, ȡֵ��Χ [-999,999,999.999999  ~  +999,999,999.999999]
};

#define PDB_TYPE_IS_VALID(type)    (((type) >= PDB_FIELD_TYPE::TYPE_BOOL && (type) <= PDB_FIELD_TYPE::TYPE_BLOB) || ((type) >= PDB_FIELD_TYPE::TYPE_REAL2  && (type) <= PDB_FIELD_TYPE::TYPE_REAL6))
#define PDB_TYPE_IS_REAL(type)     ((type) >= PDB_FIELD_TYPE::TYPE_REAL2 && (type) <= PDB_FIELD_TYPE::TYPE_REAL6)


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



#include "pdb_error_msg.h"

PdbErrorMsg* PdbErrorMsg::instance_ = nullptr;

PdbErrorMsg* PdbErrorMsg::GetInstance()
{
  if (instance_ == nullptr)
  {
    instance_ = new PdbErrorMsg();
  }

  return instance_;
}

PdbErrorMsg::~PdbErrorMsg()
{

}

const char* PdbErrorMsg::GetMsgInfo(PdbErr_t errCode)
{
  auto errIt = errMsgMap_.find(errCode);

  if (errIt != errMsgMap_.end())
  {
    return errIt->second.c_str();
  }

  return unknownMsg_.c_str();
}


PdbErrorMsg::PdbErrorMsg()
{
  unknownMsg_ = "δ֪�Ĵ�����";

#ifdef _WIN32

  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_OK, "�ɹ�"));

  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_IOERR, "I/Oʧ��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_OPENED, "�����Ѵ�"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_NOMEM, "�ڴ�����ʧ��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FILE_EXIST, "�ļ��Ѵ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FILE_READONLY, "ֻ���ļ�"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PATH_TOO_LONG, "·��̫��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_CFG_ERROR, "�������ļ�����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_USER_CFG_ERROR, "�û������ļ�����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DEVID_FILE_ERROR, "�豸�ļ�����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_IDX_FILE_ERROR, "�����ļ�����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FILE_NOT_FOUND, "�ļ�������"));

  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_FILE_NAME, "��Ч���ļ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_PARAM, "��Ч�Ĳ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_HANDLE, "��Ч�ľ��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_USER_NAME, "��Ч���û���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_USER_ROLE, "��Ч���û���ɫ"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_INT_VAL, "��Ч������ֵ"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DOUBLE_VAL, "��Ч�ĸ���ֵ"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_BLOB_VAL, "��Ч��Blobֵ"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_TSTAMP_VAL, "��Ч��ʱ���ֵ"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DATETIME_VAL, "��Ч��DateTimeֵ"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_TABLE_NAME, "��Ч�ı���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DEVID, "��Ч���豸ID"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DEVNAME, "��Ч���豸��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DEVEXPAND, "��Ч���豸��չ��Ϣ"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_FIELD_NAME, "��Ч���ֶ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_FIELD_TYPE, "��Ч���ֶ�����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DEVID_FIELD, "��Ч���豸ID�ֶ�"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_TSTAMP_FIELD, "��Ч��ʱ����ֶ�"));


  //��¼������ҳ���
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_RECORD_FAIL, "����ļ�¼"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_RECORD_EXIST, "��¼�Ѵ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_RECORD_TOO_LONG, "��¼̫��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PAGE_FILL, "����ҳ��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PAGE_ERROR, "����ҳ����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_VALUE_MISMATCH, "ֵ���Ͳ�ƥ��"));

  //����ֶ����
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FIELD_NOT_FOUND, "�ֶβ�����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FIELD_NAME_EXIST, "�ֶ�������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_NOT_FOUND, "������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_FIELD_TOO_LESS, "���ֶ�̫��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_FIELD_TOO_MANY, "���ֶ�̫��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_PART_EXIST, "���ݿ��Ѿ�����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_CAPACITY_FULL, "����������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_EXIST, "���Ѵ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DATA_FILE_IN_ACTIVE, "��Ծ�������ļ�����ɾ�������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DATA_FILE_NOT_FOUND, "�����ļ�������"));


  //���缰�������
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_NET_ERROR, "�������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_CONN_TOO_MANY, "�ͻ������ӳ�������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PASSWORD_ERROR, "�������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PACKET_ERROR, "���Ĵ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_OPERATION_DENIED, "�������ܾ�"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TASK_CANCEL, "������ȡ��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TASK_STATE_ERROR, "����״̬����"));

  //SQL���
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_LOST_ALIAS, "����ָ������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_GROUP_ERROR, "SQL�������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_GROUP_LOST_BEGIN_TSTAMP, "SQL����ȱ����ʼʱ��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_ERROR, "SQL������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_CONDITION_EXPR_ERROR, "SQL�������ʽ����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_RESULT_ERROR, "SQL���������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_RESULT_TOO_SMALL, "SQL�����̫С"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_RESULT_TOO_LARGE, "SQL�����̫��"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_LIMIT_ERROR, "SQL Limit����"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_NOT_QUERY, "���ǲ�ѯSQL"));

  //�豸���������
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_RESLT_FULL, "���������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DEVID_EXISTS, "�豸ID�Ѵ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DEV_CAPACITY_FULL, "�豸��������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_USER_EXIST, "�û��Ѵ���"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_USER_NOT_FOUND, "�û�������"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_IDX_NOT_FOUND, "����δ�ҵ�"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DEV_NOT_FOUND, "�豸δ�ҵ�"));

#else

  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_OK, "success"));

  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_IOERR, "I/O error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_OPENED, "object opened"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_NOMEM, "out of memory"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FILE_EXIST, "file exists"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FILE_READONLY, "read-only file"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PATH_TOO_LONG, "invalid path"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_CFG_ERROR, "table config error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_USER_CFG_ERROR, "user config error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DEVID_FILE_ERROR, "devid file error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_IDX_FILE_ERROR, "index file error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FILE_NOT_FOUND, "no such file"));

  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_FILE_NAME, "invalid file name"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_PARAM, "invalid param"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_HANDLE, "invalid handle"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_USER_NAME, "invalid user name"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_USER_ROLE, "invalid user role"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_INT_VAL, "invalid integer value"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DOUBLE_VAL, "invalid float value"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_BLOB_VAL, "invalid blob value"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_TSTAMP_VAL, "invalid tstamp value"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DATETIME_VAL, "invalid datetime value"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_TABLE_NAME, "invalid table name"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DEVID, "invalid device id"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DEVNAME, "invalid device name"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DEVEXPAND, "invalid device expand info"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_FIELD_NAME, "invalid field name"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_FIELD_TYPE, "invalid field type"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_DEVID_FIELD, "invalid device id field"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_INVALID_TSTAMP_FIELD, "invalid tstamp field"));


  //��¼������ҳ���
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_RECORD_FAIL, "record fail"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_RECORD_EXIST, "record exist"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_RECORD_TOO_LONG, "record too long"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PAGE_FILL, "data page fill"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PAGE_ERROR, "data page error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_VALUE_MISMATCH, "value type mismatch"));

  //����ֶ����
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FIELD_NOT_FOUND, "field not found"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_FIELD_NAME_EXIST, "field exist"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_NOT_FOUND, "table not found"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_FIELD_TOO_LESS, "field too less"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_FIELD_TOO_MANY, "field too many"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_PART_EXIST, "table part exist"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_CAPACITY_FULL, "table capacity full"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TABLE_EXIST, "table exist"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DATA_FILE_IN_ACTIVE, "data file in active"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DATA_FILE_NOT_FOUND, "data file not found"));


  //���缰�������
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_NET_ERROR, "network error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_CONN_TOO_MANY, "too many connections"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PASSWORD_ERROR, "password error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_PACKET_ERROR, "packet error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_OPERATION_DENIED, "operation denied"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TASK_CANCEL, "task cancel"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_TASK_STATE_ERROR, "task state error"));

  //SQL���
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_LOST_ALIAS, "lost alias name"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_GROUP_ERROR, "SQL group error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_GROUP_LOST_BEGIN_TSTAMP, "lost begin tstamp"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_ERROR, "SQL error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_CONDITION_EXPR_ERROR, "SQL condition error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_RESULT_ERROR, "SQL result error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_RESULT_TOO_SMALL, "SQL result too small"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_RESULT_TOO_LARGE, "SQL result too large"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_LIMIT_ERROR, "SQL limit error"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_SQL_NOT_QUERY, "not query sql"));

  //�豸���������
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_RESLT_FULL, "result full"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DEVID_EXISTS, "device exists"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DEV_CAPACITY_FULL, "device capacity full"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_USER_EXIST, "user exist"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_USER_NOT_FOUND, "user not found"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_IDX_NOT_FOUND, "index not found"));
  errMsgMap_.insert(std::make_pair<PdbErr_t, std::string>(PdbE_DEV_NOT_FOUND, "device not found"));

#endif

}


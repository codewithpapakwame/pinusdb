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

#define PdbE_OK                             0                // �ɹ�

//�ļ�ϵͳ���
#define PdbE_IOERR                          50000           // I/Oʧ��
#define PdbE_OPENED                         50001           // �����Ѵ�
#define PdbE_NOMEM                          50002           // �ڴ�����ʧ��
#define PdbE_FILE_EXIST                     50003           // �ļ��Ѵ���
#define PdbE_FILE_READONLY                  50004           // ֻ���ļ�
#define PdbE_PATH_TOO_LONG                  50005           // ·��̫��
#define PdbE_TABLE_CFG_ERROR                50006           // �������ļ�����
#define PdbE_USER_CFG_ERROR                 50007           // �û������ļ�����
#define PdbE_DEVID_FILE_ERROR               50008           // �豸�ļ�����
#define PdbE_IDX_FILE_ERROR                 50009           // �����ļ�����
#define PdbE_FILE_NOT_FOUND                 50010           // �ļ�������
#define PdbE_DATA_LOG_ERROR                 50011           // ��־�ļ�����
#define PdbE_END_OF_DATALOG                 50012           // ������־��ȡ���
#define PdbE_PATH_NOT_FOUND                 50013           // Ŀ¼������
#define PdbE_DATA_LOG_VER_ERROR             50014           // ������־�汾����
#define PdbE_DATA_FILECODE_ERROR            50015           // ������־�ļ���Ŵ���
#define PdbE_LOGFILE_FULL                   50016           // ��־�ļ���

#define PdbE_INVALID_FILE_NAME              50100           // ��Ч���ļ���
#define PdbE_INVALID_PARAM                  50101           // ��Ч�Ĳ���
#define PdbE_INVALID_HANDLE                 50102           // ��Ч�ľ��
#define PdbE_INVALID_USER_NAME              50103           // ��Ч���û���
#define PdbE_INVALID_USER_ROLE              50104           // ��Ч���û���ɫ
#define PdbE_INVALID_INT_VAL                50105           // ��Ч������ֵ
#define PdbE_INVALID_DOUBLE_VAL             50106           // ��Ч�ĸ���ֵ
#define PdbE_INVALID_BLOB_VAL               50107           // ��Ч��Blobֵ
#define PdbE_INVALID_TSTAMP_VAL             50108           // ��Ч��ʱ���ֵ
#define PdbE_INVALID_DATETIME_VAL           50109           // ��Ч��DateTimeֵ
#define PdbE_INVALID_TABLE_NAME             50110           // ��Ч�ı���
#define PdbE_INVALID_DEVID                  50111           // ��Ч���豸ID
#define PdbE_INVALID_DEVNAME                50112           // ��Ч���豸��
#define PdbE_INVALID_DEVEXPAND              50113           // ��Ч���豸��չ��Ϣ
#define PdbE_INVALID_FIELD_NAME             50114           // ��Ч���ֶ���
#define PdbE_INVALID_FIELD_TYPE             50115           // ��Ч���ֶ�����
#define PdbE_INVALID_DEVID_FIELD            50116           // ��Ч���豸ID�ֶ�
#define PdbE_INVALID_TSTAMP_FIELD           50117           // ��Ч��ʱ����ֶ�
#define PdbE_OBJECT_INITIALIZED             50118           // �����ѳ�ʼ��

//��¼������ҳ���
#define PdbE_RECORD_FAIL                    50200           // ����ļ�¼
#define PdbE_RECORD_EXIST                   50201           // ��¼�Ѵ���
#define PdbE_RECORD_TOO_LONG                50202           // ��¼̫��
#define PdbE_PAGE_FILL                      50203           // ����ҳ��
#define PdbE_PAGE_ERROR                     50204           // ����ҳ����
#define PdbE_VALUE_MISMATCH                 50205           // ֵ���Ͳ�ƥ��
#define PdbE_NULL_VALUE                     50206           // ��ֵ
#define PdbE_TSTAMP_DISORDER                50207           // tstamp����
#define PdbE_NODATA                         50208           // ȱ������
#define PdbE_COMPRESS_ERROR                 50209           // ѹ��ʧ��

//����ֶ����
#define PdbE_FIELD_NOT_FOUND                50300           // �ֶβ�����
#define PdbE_FIELD_NAME_EXIST               50301           // �ֶ�������
#define PdbE_TABLE_NOT_FOUND                50302           // ������
#define PdbE_TABLE_FIELD_TOO_LESS           50303           // ���ֶ�̫��
#define PdbE_TABLE_FIELD_TOO_MANY           50304           // ���ֶ�̫��
#define PdbE_TABLE_PART_EXIST               50305           // ���ݿ��Ѿ�����
#define PdbE_TABLE_CAPACITY_FULL            50306           // ����������
#define PdbE_TABLE_EXIST                    50307           // ���Ѵ���
#define PdbE_DATA_FILE_IN_ACTIVE            50308           // ��Ծ�������ļ�����ɾ�������
#define PdbE_DATA_FILE_NOT_FOUND            50309           // �����ļ�������
#define PdbE_TABLE_FIELD_MISMATCH           50310           // ���ֶβ�ƥ��

//���缰�������
#define PdbE_NET_ERROR                      50400           // �������
#define PdbE_CONN_TOO_MANY                  50401           // �ͻ������ӳ�������
#define PdbE_PASSWORD_ERROR                 50402           // �������
#define PdbE_PACKET_ERROR                   50403           // ���Ĵ���
#define PdbE_OPERATION_DENIED               50404           // �������ܾ�
#define PdbE_TASK_CANCEL                    50405           // ������ȡ��
#define PdbE_TASK_STATE_ERROR               50406           // ����״̬����
#define PdbE_RETRY                          50407           // �Ժ�����
#define PdbE_QUERY_TIME_OUT                 50408           // ��ѯ��ʱ
#define PdbE_NOT_LOGIN                      50409           // δ��¼
#define PdbE_INSERT_PART_ERROR              50410           // ���ֲ���ʧ��

//SQL���
#define PdbE_SQL_LOST_ALIAS                 50500           // ����ָ������
#define PdbE_SQL_GROUP_ERROR                50501           // SQL�������
#define PdbE_SQL_GROUP_LOST_BEGIN_TSTAMP    50502           // SQL����ȱ����ʼʱ��
#define PdbE_SQL_ERROR                      50503           // SQL������
#define PdbE_SQL_CONDITION_EXPR_ERROR       50504           // SQL�������ʽ����
#define PdbE_SQL_RESULT_ERROR               50505           // SQL���������
#define PdbE_SQL_RESULT_TOO_SMALL           50506           // SQL�����̫С
#define PdbE_SQL_RESULT_TOO_LARGE           50507           // SQL�����̫��
#define PdbE_SQL_LIMIT_ERROR                50508           // SQL Limit����
#define PdbE_SQL_NOT_QUERY                  50509           // ���ǲ�ѯSQL

//�豸���������
#define PdbE_RESULT_FULL                    50600           // ���������
#define PdbE_DEVID_EXISTS                   50601           // �豸ID�Ѵ���
#define PdbE_DEV_CAPACITY_FULL              50602           // �豸��������
#define PdbE_USER_EXIST                     50603           // �û��Ѵ���
#define PdbE_USER_NOT_FOUND                 50604           // �û�������
#define PdbE_IDX_NOT_FOUND                  50605           // ����δ�ҵ�
#define PdbE_DEV_NOT_FOUND                  50606           // �豸δ�ҵ�



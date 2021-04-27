package cn.pinusdb.jdbc;

import java.util.HashMap;

public class PDBErrCode {
	public static final int PdbE_OK                    = 0;     // �ɹ�
	
	public static final int PdbE_IOERR                 = 50000; // I/Oʧ��
	public static final int PdbE_OPENED                = 50001; // �����Ѵ�
	public static final int PdbE_NOMEM                 = 50002; // �ڴ�����ʧ��
	public static final int PdbE_FILE_EXIST            = 50003; // �ļ��Ѵ���
	public static final int PdbE_FILE_READONLY         = 50004; // ֻ���ļ�
	public static final int PdbE_PATH_TOO_LONG         = 50005; // ·��̫��
	public static final int PdbE_TABLE_CFG_ERROR       = 50006; // �������ļ�����
	public static final int PdbE_USER_CFG_ERROR        = 50007; // �û������ļ�����
	public static final int PdbE_DEVID_FILE_ERROR      = 50008; // �豸�ļ�����
	public static final int PdbE_IDX_FILE_ERROR        = 50009; // �����ļ�����
	public static final int PdbE_FILE_NOT_FOUND        = 50010; // �ļ�������
	public static final int PdbE_DATA_LOG_ERROR        = 50011; // ��־�ļ�����
	public static final int PdbE_END_OF_DATALOG        = 50012; // ������־��ȡ���
	public static final int PdbE_PATH_NOT_FOUND        = 50013; // Ŀ¼������
	public static final int PdbE_DATA_LOG_VER_ERROR    = 50014; // ������־�汾����
	public static final int PdbE_DATA_FILECODE_ERROR   = 50015; // ������־�ļ���Ŵ���
	
	public static final int PdbE_INVALID_FILE_NAME     = 50100; // ��Ч���ļ���
	public static final int PdbE_INVALID_PARAM         = 50101; // ��Ч�Ĳ���
	public static final int PdbE_INVALID_HANDLE        = 50102; // ��Ч�ľ��
	public static final int PdbE_INVALID_USER_NAME     = 50103; // ��Ч���û���
	public static final int PdbE_INVALID_USER_ROLE     = 50104; // ��Ч���û���ɫ
	public static final int PdbE_INVALID_INT_VAL       = 50105; // ��Ч������ֵ
	public static final int PdbE_INVALID_DOUBLE_VAL    = 50106; // ��Ч�ĸ���ֵ
	public static final int PdbE_INVALID_BLOB_VAL      = 50107; // ��Ч��Blobֵ
	public static final int PdbE_INVALID_TSTAMP_VAL    = 50108; // ��Ч��ʱ���ֵ
	public static final int PdbE_INVALID_DATETIME_VAL  = 50109; // ��Ч��DateTimeֵ
	public static final int PdbE_INVALID_TABLE_NAME    = 50110; // ��Ч�ı���
	public static final int PdbE_INVALID_DEVID         = 50111; // ��Ч���豸ID
	public static final int PdbE_INVALID_DEVNAME       = 50112; // ��Ч���豸��
	public static final int PdbE_INVALID_DEVEXPAND     = 50113; // ��Ч���豸��չ��Ϣ
	public static final int PdbE_INVALID_FIELD_NAME    = 50114; // ��Ч���ֶ���
	public static final int PdbE_INVALID_FIELD_TYPE    = 50115; // ��Ч���ֶ�����
	public static final int PdbE_INVALID_DEVID_FIELD   = 50116; // ��Ч���豸ID�ֶ�
	public static final int PdbE_INVALID_TSTAMP_FIELD  = 50117; // ��Ч��ʱ����ֶ�
	public static final int PdbE_OBJECT_INITIALIZED    = 50118; // �����ѳ�ʼ��
	
	
	    //��¼������ҳ���
	public static final int PdbE_RECORD_FAIL           = 50200; // ����ļ�¼
	public static final int PdbE_RECORD_EXIST          = 50201; // ��¼�Ѵ���
	public static final int PdbE_RECORD_TOO_LONG       = 50202; // ��¼̫��
	public static final int PdbE_PAGE_FILL             = 50203; // ����ҳ��
	public static final int PdbE_PAGE_ERROR            = 50204; // ����ҳ����
	public static final int PdbE_VALUE_MISMATCH        = 50205; // ֵ���Ͳ�ƥ��
	public static final int PdbE_NULL_VALUE            = 50206; // ��ֵ
	public static final int PdbE_TSTAMP_DISORDER       = 50207; // tstamp����
	public static final int PdbE_NODATA                = 50208; // ȱ������
	public static final int PdbE_COMPRESS_ERROR        = 50209; // ѹ��ʧ��
	
	    //����ֶ����
	public static final int PdbE_FIELD_NOT_FOUND       = 50300; // �ֶβ�����
	public static final int PdbE_FIELD_NAME_EXIST      = 50301; // �ֶ�������
	public static final int PdbE_TABLE_NOT_FOUND       = 50302; // ������
	public static final int PdbE_TABLE_FIELD_TOO_LESS  = 50303; // ���ֶ�̫��
	public static final int PdbE_TABLE_FIELD_TOO_MANY  = 50304; // ���ֶ�̫��
	public static final int PdbE_TABLE_PART_EXIST      = 50305; // ���ݿ��Ѿ�����
	public static final int PdbE_TABLE_CAPACITY_FULL   = 50306; // ����������
	public static final int PdbE_TABLE_EXIST           = 50307; // ���Ѵ���
	public static final int PdbE_DATA_FILE_IN_ACTIVE   = 50308; // ��Ծ�������ļ�����ɾ�������
	public static final int PdbE_DATA_FILE_NOT_FOUND   = 50309; // �����ļ�������
	
	    //���缰�������
	public static final int PdbE_NET_ERROR             = 50400; // �������
	public static final int PdbE_CONN_TOO_MANY         = 50401; // �ͻ������ӳ�������
	public static final int PdbE_PASSWORD_ERROR        = 50402; // �������
	public static final int PdbE_PACKET_ERROR          = 50403; // ���Ĵ���
	public static final int PdbE_OPERATION_DENIED      = 50404; // �������ܾ�
	public static final int PdbE_TASK_CANCEL           = 50405; // ������ȡ��
	public static final int PdbE_TASK_STATE_ERROR      = 50406; // ����״̬����
	public static final int PdbE_RETRY                 = 50407; // �Ժ�����
	public static final int PdbE_QUERY_TIME_OUT        = 50408; // ��ѯ��ʱ
	public static final int PdbE_NOT_LOGIN             = 50409; // δ��¼
	public static final int PdbE_INSERT_PART_ERROR     = 50410; // ���ֲ���ʧ��
	
	    //SQL���
	public static final int PdbE_SQL_LOST_ALIAS              = 50500; // ����ָ������
	public static final int PdbE_SQL_GROUP_ERROR             = 50501; // SQL�������
	public static final int PdbE_SQL_GROUP_LOST_BEGIN_TSTAMP = 50502; // SQL����ȱ����ʼʱ��
	public static final int PdbE_SQL_ERROR                   = 50503; // SQL������
	public static final int PdbE_SQL_CONDITION_EXPR_ERROR    = 50504; // SQL�������ʽ����
	public static final int PdbE_SQL_RESULT_ERROR            = 50505; // SQL���������
	public static final int PdbE_SQL_RESULT_TOO_SMALL        = 50506; // SQL�����̫С
	public static final int PdbE_SQL_RESULT_TOO_LARGE        = 50507; // SQL�����̫��
	public static final int PdbE_SQL_LIMIT_ERROR             = 50508; // SQL Limit����
	public static final int PdbE_SQL_NOT_QUERY               = 50509; // ���ǲ�ѯSQL
	
	    //�豸���������
	public static final int PdbE_RESULT_FULL                 = 50600; // ���������
	public static final int PdbE_DEVID_EXISTS                = 50601; // �豸ID�Ѵ���
	public static final int PdbE_DEV_CAPACITY_FULL           = 50602; // �豸��������
	public static final int PdbE_USER_EXIST                  = 50603; // �û��Ѵ���
	public static final int PdbE_USER_NOT_FOUND              = 50604; // �û�������
	public static final int PdbE_IDX_NOT_FOUND               = 50605; // ����δ�ҵ�
	public static final int PdbE_DEV_NOT_FOUND               = 50606; // �豸δ�ҵ�
	
	private static HashMap<Integer, String> errMsgMap_;
	
	static {
		errMsgMap_ = new HashMap<Integer, String>();
        errMsgMap_.put(PdbE_OK, "�ɹ�");
    	
        errMsgMap_.put(PdbE_IOERR, "I/Oʧ��");
        errMsgMap_.put(PdbE_OPENED, "�����Ѵ�");
        errMsgMap_.put(PdbE_NOMEM, "�ڴ�����ʧ��");
        errMsgMap_.put(PdbE_FILE_EXIST, "�ļ��Ѵ���");
        errMsgMap_.put(PdbE_FILE_READONLY, "ֻ���ļ�");
        errMsgMap_.put(PdbE_PATH_TOO_LONG, "·��̫��");
        errMsgMap_.put(PdbE_TABLE_CFG_ERROR, "�������ļ�����");
        errMsgMap_.put(PdbE_USER_CFG_ERROR, "�û������ļ�����");
        errMsgMap_.put(PdbE_DEVID_FILE_ERROR, "�豸�ļ�����");
        errMsgMap_.put(PdbE_IDX_FILE_ERROR, "�����ļ�����");
        errMsgMap_.put(PdbE_FILE_NOT_FOUND, "�ļ�������");
        errMsgMap_.put(PdbE_DATA_LOG_ERROR, "��־�ļ�����");
        errMsgMap_.put(PdbE_END_OF_DATALOG, "������־��ȡ���");
        errMsgMap_.put(PdbE_PATH_NOT_FOUND, "Ŀ¼������");
        errMsgMap_.put(PdbE_DATA_LOG_VER_ERROR, "������־�汾����");
        errMsgMap_.put(PdbE_DATA_FILECODE_ERROR, "������־�ļ���Ŵ���");
	
        errMsgMap_.put(PdbE_INVALID_FILE_NAME, "��Ч���ļ���");
        errMsgMap_.put(PdbE_INVALID_PARAM, "��Ч�Ĳ���");
        errMsgMap_.put(PdbE_INVALID_HANDLE, "��Ч�ľ��");
        errMsgMap_.put(PdbE_INVALID_USER_NAME, "��Ч���û���");
        errMsgMap_.put(PdbE_INVALID_USER_ROLE, "��Ч���û���ɫ");
        errMsgMap_.put(PdbE_INVALID_INT_VAL, "��Ч������ֵ");
        errMsgMap_.put(PdbE_INVALID_DOUBLE_VAL, "��Ч�ĸ���ֵ");
        errMsgMap_.put(PdbE_INVALID_BLOB_VAL, "��Ч��Blobֵ");
        errMsgMap_.put(PdbE_INVALID_TSTAMP_VAL, "��Ч��ʱ���ֵ");
        errMsgMap_.put(PdbE_INVALID_DATETIME_VAL, "��Ч��DateTimeֵ");
        errMsgMap_.put(PdbE_INVALID_TABLE_NAME, "��Ч�ı���");
        errMsgMap_.put(PdbE_INVALID_DEVID, "��Ч���豸ID");
        errMsgMap_.put(PdbE_INVALID_DEVNAME, "��Ч���豸��");
        errMsgMap_.put(PdbE_INVALID_DEVEXPAND, "��Ч���豸��չ��Ϣ");
        errMsgMap_.put(PdbE_INVALID_FIELD_NAME, "��Ч���ֶ���");
        errMsgMap_.put(PdbE_INVALID_FIELD_TYPE, "��Ч���ֶ�����");
        errMsgMap_.put(PdbE_INVALID_DEVID_FIELD, "��Ч���豸ID�ֶ�");
        errMsgMap_.put(PdbE_INVALID_TSTAMP_FIELD, "��Ч��ʱ����ֶ�");
        errMsgMap_.put(PdbE_OBJECT_INITIALIZED, "�����ѳ�ʼ��");
	
	
	    //��¼������ҳ���
        errMsgMap_.put(PdbE_RECORD_FAIL, "����ļ�¼");
        errMsgMap_.put(PdbE_RECORD_EXIST, "��¼�Ѵ���");
        errMsgMap_.put(PdbE_RECORD_TOO_LONG, "��¼̫��");
        errMsgMap_.put(PdbE_PAGE_FILL, "����ҳ��");
        errMsgMap_.put(PdbE_PAGE_ERROR, "����ҳ����");
        errMsgMap_.put(PdbE_VALUE_MISMATCH, "ֵ���Ͳ�ƥ��");
        errMsgMap_.put(PdbE_NULL_VALUE, "��ֵ");
        errMsgMap_.put(PdbE_TSTAMP_DISORDER, "tstamp����");
        errMsgMap_.put(PdbE_NODATA, "ȱ������");
        errMsgMap_.put(PdbE_COMPRESS_ERROR, "ѹ��ʧ��");
	
	    //����ֶ����
        errMsgMap_.put(PdbE_FIELD_NOT_FOUND, "�ֶβ�����");
        errMsgMap_.put(PdbE_FIELD_NAME_EXIST, "�ֶ�������");
        errMsgMap_.put(PdbE_TABLE_NOT_FOUND, "������");
        errMsgMap_.put(PdbE_TABLE_FIELD_TOO_LESS, "���ֶ�̫��");
        errMsgMap_.put(PdbE_TABLE_FIELD_TOO_MANY, "���ֶ�̫��");
        errMsgMap_.put(PdbE_TABLE_PART_EXIST, "���ݿ��Ѿ�����");
        errMsgMap_.put(PdbE_TABLE_CAPACITY_FULL, "����������");
        errMsgMap_.put(PdbE_TABLE_EXIST, "���Ѵ���");
        errMsgMap_.put(PdbE_DATA_FILE_IN_ACTIVE, "��Ծ�������ļ�����ɾ�������");
        errMsgMap_.put(PdbE_DATA_FILE_NOT_FOUND, "�����ļ�������");
	
	    //���缰�������
        errMsgMap_.put(PdbE_NET_ERROR, "�������");
        errMsgMap_.put(PdbE_CONN_TOO_MANY, "�ͻ������ӳ�������");
        errMsgMap_.put(PdbE_PASSWORD_ERROR, "�������");
        errMsgMap_.put(PdbE_PACKET_ERROR, "���Ĵ���");
        errMsgMap_.put(PdbE_OPERATION_DENIED, "�������ܾ�");
        errMsgMap_.put(PdbE_TASK_CANCEL, "������ȡ��");
        errMsgMap_.put(PdbE_TASK_STATE_ERROR, "����״̬����");
        errMsgMap_.put(PdbE_RETRY, "�Ժ�����");
        errMsgMap_.put(PdbE_QUERY_TIME_OUT, "��ѯ��ʱ");
        errMsgMap_.put(PdbE_NOT_LOGIN, "δ��¼");
        errMsgMap_.put(PdbE_INSERT_PART_ERROR, "���ֲ���ʧ��");
	
	    //SQL���
        errMsgMap_.put(PdbE_SQL_LOST_ALIAS, "����ָ������");
        errMsgMap_.put(PdbE_SQL_GROUP_ERROR, "SQL�������");
        errMsgMap_.put(PdbE_SQL_GROUP_LOST_BEGIN_TSTAMP, "SQL����ȱ����ʼʱ��");
        errMsgMap_.put(PdbE_SQL_ERROR, "SQL������");
        errMsgMap_.put(PdbE_SQL_CONDITION_EXPR_ERROR, "SQL�������ʽ����");
        errMsgMap_.put(PdbE_SQL_RESULT_ERROR, "SQL���������");
        errMsgMap_.put(PdbE_SQL_RESULT_TOO_SMALL, "SQL�����̫С");
        errMsgMap_.put(PdbE_SQL_RESULT_TOO_LARGE, "SQL�����̫��");
        errMsgMap_.put(PdbE_SQL_LIMIT_ERROR, "SQL Limit����");
        errMsgMap_.put(PdbE_SQL_NOT_QUERY, "���ǲ�ѯSQL");
	
	    //�豸���������
        errMsgMap_.put(PdbE_RESULT_FULL, "���������");
        errMsgMap_.put(PdbE_DEVID_EXISTS, "�豸ID�Ѵ���");
        errMsgMap_.put(PdbE_DEV_CAPACITY_FULL, "�豸��������");
        errMsgMap_.put(PdbE_USER_EXIST, "�û��Ѵ���");
        errMsgMap_.put(PdbE_USER_NOT_FOUND, "�û�������");
        errMsgMap_.put(PdbE_IDX_NOT_FOUND, "����δ�ҵ�");
        errMsgMap_.put(PdbE_DEV_NOT_FOUND, "�豸δ�ҵ�");
	}
	
	public static String errMsg(int errCode) {
		if (errMsgMap_.containsKey(errCode)) {
			return errMsgMap_.get(errCode)  + ", " + errCode;
		}
		
		return "δ֪�Ĵ���, " + errCode;
	}
}


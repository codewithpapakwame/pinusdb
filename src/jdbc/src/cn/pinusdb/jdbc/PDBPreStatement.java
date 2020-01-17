package cn.pinusdb.jdbc;

import java.io.InputStream;
import java.io.Reader;
import java.math.BigDecimal;
import java.net.URL;
import java.sql.Array;
import java.sql.Blob;
import java.sql.Clob;
import java.sql.Date;
import java.sql.NClob;
import java.sql.ParameterMetaData;
import java.sql.Ref;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.RowId;
import java.sql.SQLException;
import java.sql.SQLXML;
import java.sql.Time;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;

public class PDBPreStatement extends PDBStatement implements java.sql.PreparedStatement {
	private String rawSql_;
	private List<String> sqlPartList_;
	private HashMap<Integer, Object> sqlParamMap_;
	private int paramCnt_; //? ��������
	private final String blobCharCode = "0123456789ABCDEF";
	
	private boolean isBatch_;
	private String batchTabName_;
	private List<String> batchColList_;
	private List<Object> batchVals_;
	
	public PDBPreStatement(PDBConnection conn, String rawSql) throws SQLException {
		super(conn);
		rawSql_ = rawSql;
		sqlPartList_ = new ArrayList<String>();
		sqlParamMap_ = new HashMap<Integer, Object>();
		paramCnt_ = 0;
		isBatch_ = false;
		batchTabName_ = null;
		batchColList_ = new ArrayList<String>();
		batchVals_ = new ArrayList<Object>();
		
		if (!splitSqlForBatchInsert(rawSql)) {
			splitSql(rawSql);
		}
	}
	
	private void splitSql(String sqlStr) throws SQLException {
		int pos = getSqlBeginPos(sqlStr);
		if (pos < 0) {
			throw new SQLException(PDBErrCode.errMsg(PDBErrCode.PdbE_SQL_ERROR), 
					"58005", PDBErrCode.PdbE_SQL_ERROR);
		}
		
		sqlStr = sqlStr.substring(pos);
		
		int curIdx = 0;
		int partBg = 0;

		while(curIdx < sqlStr.length()) {
			partBg = curIdx;
			while(curIdx < sqlStr.length()) {
				char ch = sqlStr.charAt(curIdx);
				if (ch == '?' || ch == '\'' || ch == '"' || ch == '-') {
					break;
				}
				curIdx++;
			}
			
			if (curIdx == sqlStr.length()) {
				sqlPartList_.add(sqlStr.substring(partBg));
				break;
			}
			
			if (curIdx > partBg) {
				sqlPartList_.add(sqlStr.substring(partBg, curIdx));
			}
			
			partBg = curIdx;
			curIdx++;
			
			switch (sqlStr.charAt(partBg)) {
			case '?':
				sqlPartList_.add(sqlStr.substring(partBg, curIdx));
				paramCnt_++;
				break;
			case '\'':
				curIdx = sqlStr.indexOf('\'', curIdx);
				if (curIdx < 0) {
					throw new SQLException(PDBErrCode.errMsg(PDBErrCode.PdbE_SQL_ERROR), 
						"58005", PDBErrCode.PdbE_SQL_ERROR);
				}
				
				curIdx++;
				sqlPartList_.add(sqlStr.substring(partBg, curIdx));
				break;
			case '"':
				curIdx = sqlStr.indexOf('"', curIdx);
				if (curIdx < 0) {
					throw new SQLException(PDBErrCode.errMsg(PDBErrCode.PdbE_SQL_ERROR), 
						"58005", PDBErrCode.PdbE_SQL_ERROR);
				}
				
				curIdx++;
				sqlPartList_.add(sqlStr.substring(partBg, curIdx));
				break;
			case '-':
				if (curIdx >= sqlStr.length()) {
					throw new SQLException(PDBErrCode.errMsg(PDBErrCode.PdbE_SQL_ERROR), 
						"58005", PDBErrCode.PdbE_SQL_ERROR);
				}
				
				if (sqlStr.charAt(curIdx) == '-') {
					curIdx = sqlStr.indexOf('\n', curIdx);
					if (curIdx < 0) {
						curIdx = sqlStr.length();
					} else {
						curIdx++;
					}
				}
				
				sqlPartList_.add(sqlStr.substring(partBg, curIdx));
				break;
			}
			
		}
		
	}

	private boolean splitSqlForBatchInsert(String sqlStr) {
		List<SqlPart> partList = new ArrayList<SqlPart>();
		final String preInsert = "insert";
		final String preInto = "into";
		final String preValues = "values";
		int curIdx = 0;
		int partBg = 0;
		char ch ;
		
		sqlStr = sqlStr.toLowerCase();
		
		while(curIdx < sqlStr.length()) {
			partBg = curIdx;
			
			//�������п��ַ�---BEGIN--------------
			while (curIdx < sqlStr.length()) {
			    ch = sqlStr.charAt(curIdx);
				if (ch != ' ' && ch != '\n' && ch != '\t' && ch != '\f' && ch != '\r') {
					break;
				}
				
				curIdx++;
			}
			
			if (curIdx != partBg) {
				continue;
			}
			//�������п��ַ�---END----------------
			
			//��������ע��---BEGIN----------------
			if (sqlStr.startsWith("--", curIdx))
			{
				curIdx = sqlStr.indexOf('\n', curIdx);
				if (curIdx < 0) {
					break;
				}
				continue;
			}
			//��������ע��---END------------------
		
			//�������йؼ���---BEGIN---------insert into values
			if (sqlStr.startsWith(preInsert, curIdx)) {
				partList.add(new SqlPart("insert", SqlPartType.TK_INSERT));
				curIdx += preInsert.length();
				continue;
			} else if (sqlStr.startsWith(preInto, curIdx)) {
				partList.add(new SqlPart("into", SqlPartType.TK_INTO));
				curIdx += preInto.length();
				continue;
			} else if (sqlStr.startsWith(preValues, curIdx)) {
				partList.add(new SqlPart("values", SqlPartType.TK_VALUES));
				curIdx += preValues.length();
				continue;
			}
			//�������йؼ���---END---------insert into values
			
			//���������ַ�---BEGIN------------,()?
			ch = sqlStr.charAt(curIdx);
			if (ch == ',') {
				partList.add(new SqlPart(",", SqlPartType.TK_COMMA));
				curIdx++;
				continue;
			} else if (ch == '(') {
				partList.add(new SqlPart("(", SqlPartType.TK_LP));
				curIdx++;
				continue;
			} else if (ch == ')') {
				partList.add(new SqlPart(")", SqlPartType.TK_RP));
				curIdx++;
				continue;
			} else if (ch == '?') {
				partList.add(new SqlPart("?", SqlPartType.TK_PARAM));
				curIdx++;
				continue;
			}
			//���������ַ�---END------------,()?
			
			//�����ʶ��---BEGIN-----------------------------
			if (ch == '`') {
				curIdx++;
				partBg = curIdx;
				curIdx = sqlStr.indexOf('`', curIdx);
				if (curIdx < 0) {
					return false;
				}
				
				partList.add(new SqlPart(sqlStr.substring(partBg, curIdx), SqlPartType.TK_ID));
				curIdx++;
				continue;
			} else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
				
				while((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') 
					|| (ch >= '0' && ch <= '9') || ch == '_') {
					curIdx++;
					if (curIdx >= sqlStr.length())
						break;
					
					ch = sqlStr.charAt(curIdx);
				}
				
				partList.add(new SqlPart(sqlStr.substring(partBg, curIdx), SqlPartType.TK_ID));
				continue;
			}
			//�����ʶ��---END-------------------------------
			
			return false;
		}
		
		//�ж��Ƿ�������������Ҫ��
		//��֤INSERT INTO tab(col1
		if (partList.size() < 5)
			return false;
		if (partList.get(0).getPartType().getTypeVal() != SqlPartType.TK_INSERT.getTypeVal()
			|| partList.get(1).getPartType().getTypeVal() != SqlPartType.TK_INTO.getTypeVal()
			|| partList.get(2).getPartType().getTypeVal() != SqlPartType.TK_ID.getTypeVal()
			|| partList.get(3).getPartType().getTypeVal() != SqlPartType.TK_LP.getTypeVal()
			|| partList.get(4).getPartType().getTypeVal() != SqlPartType.TK_ID.getTypeVal()) {
			return false;
		}
		
		batchTabName_ = partList.get(2).getPartStr();
		
		int partIdx = 4;
		while (true) {
			if (partIdx + 2 >= partList.size()) {
				return false;
			}
			
			if (partList.get(partIdx).getPartType().getTypeVal() != SqlPartType.TK_ID.getTypeVal()) {
				return false;
			}
			
			batchColList_.add(partList.get(partIdx).getPartStr());
			partIdx++;
			if (partList.get(partIdx).getPartType().getTypeVal() != SqlPartType.TK_COMMA.getTypeVal()) {
				break;
			}
			
			partIdx++;
		}
		
		//��֤ )values(
		if (partIdx + 3 >= partList.size()) {
			return false;
		}
		
		if (partList.get(partIdx).getPartType().getTypeVal() != SqlPartType.TK_RP.getTypeVal()
			|| partList.get(partIdx + 1).getPartType().getTypeVal() != SqlPartType.TK_VALUES.getTypeVal()
			|| partList.get(partIdx + 2).getPartType().getTypeVal() != SqlPartType.TK_LP.getTypeVal()) {
			return false;
		}
		
		//��ȡ?
		int paramCnt = 0;
		partIdx += 3;
		while (true) {
			if (partIdx + 2 > partList.size()) {
				return false;
			}
			
			if (partList.get(partIdx).getPartType().getTypeVal() != SqlPartType.TK_PARAM.getTypeVal()) {
				return false;
			}
			
			paramCnt++;
			partIdx++;
			if (partList.get(partIdx).getPartType().getTypeVal() != SqlPartType.TK_COMMA.getTypeVal()) {
				break;
			}
			
			partIdx++;
		}
		
		if (partIdx + 1 != partList.size()) {
			return false;
		}
		
		if (partList.get(partIdx).getPartType().getTypeVal() != SqlPartType.TK_RP.getTypeVal()) {
			return false;
		}
		
		if (paramCnt != batchColList_.size()) {
			return false;
		}
		
		isBatch_ = true;
		paramCnt_ = batchColList_.size();
		return true;
	}
	
	private String BuildSql() throws SQLException {
		if (paramCnt_ == 0) {
			return rawSql_;
		}
		
		if (sqlParamMap_.size() != paramCnt_) {
			throw new SQLException("ȱ�ٲ���", "58005", PDBErrCode.PdbE_INVALID_PARAM);
		}
		
		StringBuilder sqlBuilder = new StringBuilder(1024);
		int paramPos = 1;
		for(String part:sqlPartList_) {
			if (part.compareTo("?") == 0) {
				sqlBuilder.append(ObjectValToString(sqlParamMap_.get(paramPos)));
				paramPos++;
			} else {
				sqlBuilder.append(part);
			}
		}
		
		return sqlBuilder.toString();
	}
	
	private String ObjectValToString(Object val) throws SQLException {
		if (val instanceof Boolean) {
			return ((Boolean)val) ? "true" : "false";
		} else if (val instanceof Long || val instanceof Float || val instanceof Double) {
			return val.toString();
		} else if (val instanceof Timestamp) {
			DateFormat tmFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
			return "'" + tmFormat.format(val) + "'";
		} else if (val instanceof String) {
			return "'" + ((String)val).replace("'", "''") + "'";
		} else if (val instanceof byte[]) {
			byte[] byteVals = (byte[])val;
			StringBuilder blobBuilder = new StringBuilder(byteVals.length * 2 + 3);
			blobBuilder.append("x'");
			for (byte b : byteVals) {
				blobBuilder.append(blobCharCode.charAt((b >> 4) & 0xF));
				blobBuilder.append(blobCharCode.charAt(b & 0xF));
			}
			blobBuilder.append("'");
			return blobBuilder.toString();
		}
		
		throw new SQLException("��������", "58005", PDBErrCode.PdbE_INVALID_PARAM);
	}
	
	
	@Override
	public void addBatch() throws SQLException {
		if (!isBatch_) {
			throw new SQLException("��ǰSQL��������������", "58005", PDBErrCode.PdbE_INVALID_PARAM);
		}
		
		if (sqlParamMap_.size() != paramCnt_) {
			throw new SQLException("ȱ�ٲ���", "58005", PDBErrCode.PdbE_INVALID_PARAM); 
		}
		
		for (int pos = 1; pos <= paramCnt_; pos++) {
			batchVals_.add(sqlParamMap_.get(pos));
		}
		sqlParamMap_.clear();
	}

	@Override
	public void clearParameters() throws SQLException {
		batchVals_.clear();
		sqlParamMap_.clear();
	}

	@Override
	public boolean execute() throws SQLException {
		String sql = BuildSql();
		return super.execute(sql);
	}
	
	@Override
	public int[] executeBatch() throws SQLException {
		//throw new UnsupportedOperationException("��֧�ֵķ���");
		if (!isBatch_) {
			throw new SQLException("��ǰSQL��������������", "58005", PDBErrCode.PdbE_INVALID_PARAM);
		}
		
		if (batchTabName_.length() >= 48) {
			throw new SQLException(PDBErrCode.errMsg(PDBErrCode.PdbE_INVALID_TABLE_NAME), 
				"58005", PDBErrCode.PdbE_INVALID_TABLE_NAME);
		}
		
		if (batchVals_.size() == 0) {
			throw new SQLException("ȱ�ٲ���", "58005", PDBErrCode.PdbE_INVALID_PARAM);
		}
		
		return super.batchInsert(batchTabName_, batchColList_, batchVals_);
	}

	@Override
	public ResultSet executeQuery() throws SQLException {
		String sql = BuildSql();
		return super.executeQuery(sql);
	}

	@Override
	public int executeUpdate() throws SQLException {
		String sql = BuildSql();
		return super.executeUpdate(sql);
	}

	@Override
	public ResultSetMetaData getMetaData() throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public ParameterMetaData getParameterMetaData() throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���");
	}

	@Override
	public void setArray(int parameterIndex, Array x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setAsciiStream(int parameterIndex, InputStream x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setAsciiStream(int parameterIndex, InputStream x, int length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setAsciiStream(int parameterIndex, InputStream x, long length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBigDecimal(int parameterIndex, BigDecimal x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBinaryStream(int parameterIndex, InputStream x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBinaryStream(int parameterIndex, InputStream x, int length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBinaryStream(int parameterIndex, InputStream x, long length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBlob(int parameterIndex, Blob x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBlob(int parameterIndex, InputStream inputStream) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBlob(int parameterIndex, InputStream inputStream, long length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBoolean(int parameterIndex, boolean x) throws SQLException {
		setObject(parameterIndex, (Object)x);
	}

	@Override
	public void setByte(int parameterIndex, byte x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setBytes(int parameterIndex, byte[] x) throws SQLException {
		setObject(parameterIndex, x);
	}

	@Override
	public void setCharacterStream(int parameterIndex, Reader reader) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setCharacterStream(int parameterIndex, Reader reader, int length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setCharacterStream(int parameterIndex, Reader reader, long length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setClob(int parameterIndex, Clob x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setClob(int parameterIndex, Reader reader) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setClob(int parameterIndex, Reader reader, long length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setDate(int parameterIndex, Date x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setDate(int parameterIndex, Date x, Calendar cal) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setDouble(int parameterIndex, double x) throws SQLException {
		setObject(parameterIndex, x);
	}

	@Override
	public void setFloat(int parameterIndex, float x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���");
	}

	@Override
	public void setInt(int parameterIndex, int x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setLong(int parameterIndex, long x) throws SQLException {
		setObject(parameterIndex, x);
	}

	@Override
	public void setNCharacterStream(int parameterIndex, Reader value) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setNCharacterStream(int parameterIndex, Reader value, long length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setNClob(int parameterIndex, NClob value) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setNClob(int parameterIndex, Reader reader) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setNClob(int parameterIndex, Reader reader, long length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setNString(int parameterIndex, String value) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setNull(int parameterIndex, int sqlType) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setNull(int parameterIndex, int sqlType, String typeName) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setObject(int parameterIndex, Object x) throws SQLException {
		if (parameterIndex < 1 || parameterIndex > paramCnt_) {
			throw new SQLException(PDBErrCode.errMsg(PDBErrCode.PdbE_INVALID_PARAM),
				"58005", PDBErrCode.PdbE_INVALID_PARAM);
		}
		
		if (x == null) {
			throw new SQLException(PDBErrCode.errMsg(PDBErrCode.PdbE_INVALID_PARAM),
				"58005", PDBErrCode.PdbE_INVALID_PARAM);
		}
		
		if (x instanceof Long
			|| x instanceof Boolean
			|| x instanceof Timestamp
			|| x instanceof Double
			|| x instanceof String
			|| x instanceof byte[]) {
			
			if (sqlParamMap_.containsKey(parameterIndex))
				sqlParamMap_.remove(parameterIndex);
			
			sqlParamMap_.put(parameterIndex, x);
		} else {
			throw new SQLException(PDBErrCode.errMsg(PDBErrCode.PdbE_INVALID_PARAM),
				"58005", PDBErrCode.PdbE_INVALID_PARAM);
		}
	}

	@Override
	public void setObject(int parameterIndex, Object x, int targetSqlType) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setObject(int parameterIndex, Object x, int targetSqlType, int scaleOrLength) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setRef(int parameterIndex, Ref x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setRowId(int parameterIndex, RowId x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setSQLXML(int parameterIndex, SQLXML xmlObject) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setShort(int parameterIndex, short x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setString(int parameterIndex, String x) throws SQLException {
		setObject(parameterIndex, x);
	}

	@Override
	public void setTime(int parameterIndex, Time x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setTime(int parameterIndex, Time x, Calendar cal) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setTimestamp(int parameterIndex, Timestamp x) throws SQLException {
		setObject(parameterIndex, x);
	}

	@Override
	public void setTimestamp(int parameterIndex, Timestamp x, Calendar cal) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setURL(int parameterIndex, URL x) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public void setUnicodeStream(int parameterIndex, InputStream x, int length) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}
}

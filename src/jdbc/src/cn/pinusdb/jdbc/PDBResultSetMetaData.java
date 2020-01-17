package cn.pinusdb.jdbc;

import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.List;

public class PDBResultSetMetaData implements ResultSetMetaData {

	private List<ColumnInfo> colInfoList_;
	
	PDBResultSetMetaData(List<ColumnInfo> colInfoList) {
		colInfoList_ = colInfoList;
	}
	
	@Override
	public boolean isWrapperFor(Class<?> arg0) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public <T> T unwrap(Class<T> arg0) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public java.lang.String getCatalogName(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public java.lang.String getColumnClassName(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public int getColumnCount() throws SQLException {
		return colInfoList_.size();
	}

	@Override
	public int getColumnDisplaySize(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public String getColumnLabel(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public String getColumnName(int column) throws SQLException {
		if (column < 1 || column > colInfoList_.size())
			throw new SQLException("��������");
		
		return colInfoList_.get((column - 1)).getColName();
	}

	@Override
	public int getColumnType(int column) throws SQLException {
		if (column < 1 || column > colInfoList_.size())
			throw new SQLException("��������");
		
		PDBDataType colType = colInfoList_.get((column - 1)).getColType();
		switch(colType.getTypeVal()) {
		case 1:
			return java.sql.Types.BOOLEAN;
		case 2:
			return java.sql.Types.BIGINT;
		case 3:
			return java.sql.Types.TIMESTAMP;
		case 4:
			return java.sql.Types.DOUBLE;
		case 5:
			return java.sql.Types.VARCHAR;
		case 6:
			return java.sql.Types.BLOB;
		}
		
		throw new SQLException("���������");
	}

	@Override
	public String getColumnTypeName(int column) throws SQLException {
		if (column < 1 || column > colInfoList_.size())
			throw new SQLException("��������");

		PDBDataType colType = colInfoList_.get((column - 1)).getColType();
		switch(colType.getTypeVal()) {
		case 1:
			return "bool";
		case 2:
			return "bigint";
		case 3:
			return "timestamp";
		case 4:
			return "float";
		case 5:
			return "double";
		case 6:
			return "string";
		case 7:
			return "blob";
		}
		
		throw new SQLException("���������");
	}

	@Override
	public int getPrecision(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public int getScale(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public String getSchemaName(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public String getTableName(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public boolean isAutoIncrement(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public boolean isCaseSensitive(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public boolean isCurrency(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public boolean isDefinitelyWritable(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public int isNullable(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public boolean isReadOnly(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public boolean isSearchable(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public boolean isSigned(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

	@Override
	public boolean isWritable(int column) throws SQLException {
		throw new UnsupportedOperationException("��֧�ֵķ���"); 
	}

}


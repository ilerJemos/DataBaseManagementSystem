#include "StdAfx.h"
#include "TableEntity.h"


CTableEntity::CTableEntity(void)
{
	m_strName = _T("");			// Table name
	m_nRecordNum = 0;			// Record number
	m_strTdfPath = _T("");		// Tath of table definition file 
	m_strTrdPath = _T("");		// Path of record file
	::GetLocalTime(&m_tCrTime);	// Table creation time
	m_tMTime = m_tCrTime;		// Last modification time
}


CTableEntity::CTableEntity(CString strName)
{
	m_strName = strName;		// Table name
	m_nRecordNum = 0;			// Record number
	m_strTdfPath = _T("");		// Tath of table definition file 
	m_strTrdPath = _T("");		// Path of record file
	::GetLocalTime(&m_tCrTime);	// Table creation time
	m_tMTime = m_tCrTime;		// Last modification time
}


CTableEntity::~CTableEntity(void)
{
	// Empty field list, free memory.
	int nCount = m_arrFields.GetCount();
	for (int i = 0;  i < nCount; i++)
	{
		CFieldEntity* pField = m_arrFields.GetAt(i);
		if (pField != NULL)
		{
			delete pField;
			pField = NULL;
		}
	}
}

TableBlock CTableEntity::GetBlock()
{
	TableBlock tb;
	CCharHelper::ToChars(tb.name, m_strName, sizeof(VARCHAR));
	tb.field_num = m_arrFields.GetCount();
	tb.record_num = m_nRecordNum;
	CCharHelper::ToChars(tb.tdf, m_strTdfPath, sizeof(VARCHAR));
	CCharHelper::ToChars(tb.trd, m_strTrdPath, sizeof(VARCHAR));
	tb.crtime = m_tCrTime;
	tb.mtime = m_tMTime;

	return tb;
}


void CTableEntity::SetBlock(TableBlock tb)
{
	m_strName = CCharHelper::ToString(tb.name, sizeof(VARCHAR));
	m_nRecordNum = tb.record_num;
	m_strTdfPath = CCharHelper::ToString(tb.tdf, sizeof(VARCHAR));
	m_strTrdPath = CCharHelper::ToString(tb.trd, sizeof(VARCHAR));
	m_tCrTime = tb.crtime;
	m_tMTime = tb.mtime;
}

// Set function
void CTableEntity::SetName(CString strName)
{
	m_strName = strName;
}

void CTableEntity::SetRecordNum(int nNum)
{
	m_nRecordNum = nNum;
}

void CTableEntity::SetTdfPath( const CString strTdfPath )
{
	m_strTdfPath = strTdfPath;
}

void CTableEntity::SetTrdPath( const CString strTrdPath )
{
	m_strTrdPath = strTrdPath;
}

void CTableEntity::SetCrTime(SYSTEMTIME tTime)
{
	m_tCrTime = tTime;
}

void CTableEntity::SetMTime(SYSTEMTIME tTime)
{
	m_tMTime = tTime;
}

// Get function
CString CTableEntity::GetName()
{
	return m_strName;
}

int CTableEntity::GetRecordNum()
{
	return m_nRecordNum;
}

int CTableEntity::GetFieldNum()
{
	return m_arrFields.GetCount();
}

CString CTableEntity::GetTdfPath()
{
	return m_strTdfPath;
}

CString CTableEntity::GetTrdPath()
{
	return m_strTrdPath;
}

SYSTEMTIME CTableEntity::GetCrTime()
{
	return m_tCrTime;
}

SYSTEMTIME CTableEntity::GetMTime()
{
	return m_tMTime;
}


CFieldEntity* CTableEntity::AddField(CFieldEntity &fe)
{
	// Add field
	CFieldEntity* pField = new CFieldEntity(fe);
	m_arrFields.Add(pField);

	return pField;
}


CFieldEntity* CTableEntity::GetFieldAt(int nIndex)
{
	return m_arrFields.GetAt(nIndex);
}
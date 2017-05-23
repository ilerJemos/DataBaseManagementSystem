#include "StdAfx.h"
#include "DBEntity.h"


CDBEntity::CDBEntity(void)
{
	m_strName = _T("");
	m_bType = false;
	m_strFilepath = _T("");
	::GetLocalTime(&m_tCtTime);
}


CDBEntity::CDBEntity(const CString strName)
{
	m_strName = strName;
	m_bType = false;
	m_strFilepath = _T("");
	::GetLocalTime(&m_tCtTime);
}

CDBEntity::CDBEntity(DatabaseBlock &db)
{
	this->SetBlock(db);
}


CDBEntity::~CDBEntity(void)
{

}


DatabaseBlock CDBEntity::GetBlock(void)
{
	DatabaseBlock db;
	memset(&db, 0, sizeof(DatabaseBlock));// Initializes the memory space

	CCharHelper::ToChars(db.filepath, m_strFilepath, sizeof(VARCHAR));
	CCharHelper::ToChars(db.name, m_strName, sizeof(VARCHAR));
	db.crtime = m_tCtTime;
	db.type = m_bType;

	return db;
}


void CDBEntity::SetBlock(DatabaseBlock db)
{
	m_strName = CCharHelper::ToString(db.name, sizeof(VARCHAR));
	m_strFilepath = CCharHelper::ToString(db.filepath, sizeof(VARCHAR));
	m_bType = db.type;
	m_tCtTime = db.crtime;
}


void CDBEntity::SetName(CString strName)
{
	m_strName = strName;
}

void CDBEntity::SetType(bool bType)
{
	m_bType = bType;
}


void CDBEntity::SetFilepath(CString strFilepath)
{
	m_strFilepath = strFilepath;
}

void CDBEntity::SetCtTime(SYSTEMTIME tTime)
{
	m_tCtTime = tTime;
}


CString CDBEntity::GetName()
{
	return m_strName;
}


bool CDBEntity::GetType()
{
	return m_bType;
}


CString CDBEntity::GetFilepath()
{
	return m_strFilepath;
}


SYSTEMTIME CDBEntity::GetCtTime()
{
	return m_tCtTime;
}
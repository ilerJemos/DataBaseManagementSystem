#include "StdAfx.h"
#include "DBLogic.h"


bool CDBLogic::CreateDatabase(CDBEntity &db)
{
	try 
	{
		// Decide whether the file exists, if there is no,a file will be created.
		CString strDBFile = m_fileLogic.GetDBFile();
		if (CFileHelper::IsValidFile(strDBFile) == false)
		{
			if(CFileHelper::CreateFile(strDBFile) == false)
			{
				return false;
			}
		}

		// Read file, decide whether exists the database of the same name
		if (m_daoDB.GetDatabase(strDBFile, db) == true)
		{
			return false;
		}

		// Create database
		CString strDBFolder = m_fileLogic.GetDBFolder(db.GetName());
		db.SetFilepath(strDBFolder);
		if(m_daoDB.Create(strDBFile, db) == false)
		{
			return false;
		}
	}
	catch (CAppException* e)
	{
		throw e;
	}
	catch (...)
	{
		throw new CAppException(_T("Failed to create database!"));
	}

	return true;
}

bool CDBLogic::GetDatabase(CDBEntity &db)
{
	try
	{
		return m_daoDB.GetDatabase(m_fileLogic.GetDBFile(), db);
	}
	catch (CAppException* e)
	{
		throw e;
	}
	catch (...)
	{
		throw new CAppException(_T("Failed to create database!"));
	}
	return false;
}
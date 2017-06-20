#include "StdAfx.h"
#include "DBLogic.h"
#include"AppException.h"

/**************************************************
[FunctionName]	CreateDatabase
[Function]	Create database
[Argument]	CDBEntity &db: Database entity object, contains the name of the database.
[ReturnedValue]	bool: True if the operation is successful, other false.
**************************************************/
bool CDBLogic::CreateDatabase(CDBEntity &db)
{
	try 
	{
		// Decide whether the file exists, if there is no,a file will be created.
		CString strDBFile = m_fileLogic.GetDBFile();
		if (m_daoDB.IsValidFile(strDBFile) == false)
		{
			if(m_daoDB.CreateFile(strDBFile) == false)
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

/**************************************************
[FunctionName]	GetDatabase
[Function]	Query the database information
[Argument]	CDBEntity &db: Database entity object, contains the name of the database.
		Queried database information will be saved to the object.
[ReturnedValue]	bool: True if exists the database of the same name, otherwise false
**************************************************/
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
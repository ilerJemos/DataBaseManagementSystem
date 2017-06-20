#include "StdAfx.h"
#include "DBDao.h"
#include"AppException.h"
#include"CharHelper.h"

/**************************************************
[FunctionName] Create
[Function] Create database, insert a record into ruanko.db file.
[Argument]	const CString strFilepath: The database file path
		CDBEntity db: The database entities, including the database name.
		bool bAppend: Whether write file in a additional way.true if additional and false if overwritten
[ReturnedValue]	bool: True if the operation is successful; otherwise false.
**************************************************/
bool CDBDao::Create(const CString strFilepath, CDBEntity db, bool bAppend)
{
	try
	{
		CFile file;
		if(bAppend == true)
		{
			if (file.Open(strFilepath, CFile::modeWrite | CFile::shareDenyWrite) == FALSE)
			{
				return false;
			}
		}
		else
		{
			if (file.Open(strFilepath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite) == FALSE)
			{
				return false;
			}
		}

		file.SeekToBegin();
		file.Write(&db.GetBlock(), sizeof(DatabaseBlock));
		file.Close();

		return true;
	}
	catch(CException* e)// Catch exceptions
	{
		// Delete the exception information
		e->Delete();
		// Throw a custom exception
		throw new CAppException(_T("Failed to create the database file!"));
	}
	catch(...)	// Catch other exceptions
	{
		// Throw a custom exception
		throw new CAppException(_T("Failed to create the database file!"));
	}

	return false;
}

/**************************************************
[FunctionName]	GetDatabase
[Function]	Read the database information
[Argument]	const CString strFilepath: The database file path
		CDBEntity db: The database entities, including the database name.
[ReturnedValue]	bool: True if the operation is successful; otherwise false.
**************************************************/
bool CDBDao::GetDatabase(const CString strFilepath, CDBEntity &db)
{
	try
	{
		// Verify the file name
		CString strDBName = db.GetName();
		if (strDBName.GetLength() == 0)
		{
			return false;
		}
		
		// Open the file in a read-only way
		CFile file;
		if (file.Open(strFilepath, CFile::modeRead | CFile::shareDenyNone) == FALSE)
		{
			return false;
		}

		// Read the database information
		bool bExist = false;
		DatabaseBlock sdb;

		file.SeekToBegin();
		while (file.Read(&sdb, sizeof(DatabaseBlock)) > 0)
		{
			CString strName = CCharHelper::ToString(sdb.name, sizeof(VARCHAR));
			if (strName.CompareNoCase(strDBName) == 0)
			{
				db.SetBlock(sdb);
				bExist = true;
				break;
			}
		}
		file.Close();

		return bExist;
	}
	catch(CException* e)// Catch exceptions
	{
		// Delete the exception information
		e->Delete();
		// Throw a custom exception
		throw new CAppException(_T("Failed to create the database file!"));
	}
	catch(...)	// Catch other exceptions
	{
		// Throw a custom exception
		throw new CAppException(_T("Failed to create the database file!"));
	}
	return false;
}


/***************************************************************************
[FunctionName]	CreateFile
[Function]	Create a file, and create folders when file path of the folder does not exist.
[Argument]	strFileName:File path
[ReturnedValue]	bool:Ture if the operation is successful,otherwise the false
***************************************************************************/
bool CDBDao::CreateFile(const CString strFileName)
{
	try
	{
		// Create all the folder of the fiel path
		for (int i = 0; i < strFileName.GetLength(); i++)
		{
			if ( (_T('\\') == strFileName.GetAt(i)
				|| _T('/') == strFileName.GetAt(i))
				&& 2 != i
				)
			{
				CString strDirectory;
				strDirectory = strFileName.Left(i);
				if(!CreateDirectory(strDirectory, NULL) && 183 != GetLastError())
				{
					return false;
				}
			}
		}

		// Create file
		CFile file;
		if (!file.Open(strFileName, CFile::modeCreate))
		{
			return false;
		}
		file.Close();

		return true;
	}
	catch (CException* e)
	{
		e->Delete();
		throw new CAppException(_T("Failed to create file!"));
	}
	catch (...)
	{
		throw new CAppException(_T("Failed to create file!"));
	}

	return false;
}

/************************************************************************
[FunctionName]	IsValidFile
[Function]	Decide whether the file is effective
[Argument]	CString£ºfile path
[ReturnedValue]	bool: True if the file is effective, otherwise false
************************************************************************/
bool CDBDao::IsValidFile(const CString strPath)
{
	try
	{
		CFile file;
		// Open file
		if (file.Open(strPath, CFile::modeRead | CFile::shareDenyNone) == TRUE)
		{
			// Close file
			file.Close();
			return true;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		throw new CAppException(_T("Failed to open file"));
	}
	catch (...)
	{
		throw new CAppException(_T("Failed to open file"));
	}
	return false;
}
#include "StdAfx.h"
#include "FileLogic.h"
#include"AppException.h"

/**************************************************
[FunctionName]	GetDBFile
[Function]	Get the path of the database file: ruanko.db file
[Argument]	void
[ReturnedValue]	CString: "ruanko.db",file's absolute path
**************************************************/
CString CFileLogic::GetDBFile(void)
{
	CString strPath = _T("");
	try
	{
		// Get absolute path of ruanko.db file
		strPath = GetAbsolutePath(_T("ruanko.db"));
	}
	catch (CAppException* e)
	{
		throw e;
	}
	return strPath;
}

/**************************************************
[FunctionName]	GetDBFolder
[Function]	Get the path of the database folder
[Argument]	const CString strDBName: Database name
[ReturnedValue]	CString: Absolute path of the database folder
**************************************************/
CString CFileLogic::GetDBFolder(const CString strDBName)
{
	CString strFolder = _T("");
	try
	{
		// Get the path of the database folder
		strFolder.Format(_T("data\\%s"), strDBName);
		strFolder = GetAbsolutePath(strFolder);
	}
	catch (CAppException* e)
	{
		throw e;
	}
	catch (...)
	{
		throw new CAppException(_T("Failed to get the path of the database folder!"));
	}

	return strFolder;
}

/************************************************************************
[FunctionName]	GetAbsolutePath
[Function]	Convert relative path into an absolute path.
[Argument]	CString£ºRelative paths(relative executable file in folder)
[ReturnedValue]	CString£ºAbsolute path
************************************************************************/
CString CFileLogic::GetAbsolutePath(const CString strRelativePath)
{
	CString strFolder = _T("");
	try
	{
		// Get the absolute path of the executable file
		wchar_t acExeFullPath[MAX_PATH];
		::GetModuleFileName(NULL, acExeFullPath, MAX_PATH);
		CString strFolder = acExeFullPath;
		// Get the folder path of the executable file
		int pos = strFolder.ReverseFind('\\');
		strFolder = strFolder.Left(pos+1);
		// Return absolute path
		return strFolder + strRelativePath;
	}
	catch (CException* e)
	{
		e->Delete();
		throw new CAppException(_T("Failed to get the path of file!"));
	}
	catch (...)
	{
		throw new CAppException(_T("Failed to get the path of file!"));
	}

	return strFolder;
}
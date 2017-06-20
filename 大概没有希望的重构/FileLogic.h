#pragma once

/**************************************************
[ClassName]	CFileLogic
[Function]	Database file logic class
**************************************************/
class CFileLogic
{
public:
	// Get the path of the database description file
	CString GetDBFile(void);
	// Get the path of the database folder 
	CString GetDBFolder(const CString strDBName);

private:
	// Convert relative path into an absolute path.
	CString GetAbsolutePath(const CString strRelativePath);
};
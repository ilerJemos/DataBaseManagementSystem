#pragma once

class CFileHelper
{
public:
	// Create file
	static bool CreateFile(const CString strFileName);
	// Decide the effectiveness of the file
	static bool IsValidFile(const CString strPath);
};


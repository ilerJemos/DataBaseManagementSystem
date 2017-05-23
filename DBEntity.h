#pragma once


class CDBEntity
{
public:
	// Initialize the data members
	CDBEntity(void);
	// Use DatabaseBlock structure to initialize the data members
	CDBEntity(DatabaseBlock &db);
	// Initialize the database name
	CDBEntity(const CString strName);
	// Destructor
	~CDBEntity(void);

public:
	// Class and block conversion function
	DatabaseBlock GetBlock(void);
	void SetBlock(DatabaseBlock db);

	// Set function
	void SetName(CString strName);
	void SetType(bool bType);
	void SetFilepath(CString strFilepath);
	void SetCtTime(SYSTEMTIME tTime);	

	// Get function
	CString GetName(void);
	bool GetType(void);
	CString GetFilepath(void);
	SYSTEMTIME GetCtTime(void);

private:
	CString m_strName;		// Database name
	bool m_bType;			// Database type
	CString m_strFilepath;	// The database data file path
	SYSTEMTIME m_tCtTime;	// Creation time
};
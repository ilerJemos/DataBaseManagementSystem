#pragma once
#include "afx.h"


class CAppException
{
public:
	//	Non-argument constructor
	CAppException(void);
	//	Overloaded constructor
	CAppException(CString strError);
	//	Destructor
	~CAppException(void);

public:
	//	Overload GetErrorMessage() function£¬Get error message
	CString GetErrorMessage();

private:
	CString m_strError;	//	Error message
	int m_nCode;		//	Error number
};


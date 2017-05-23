#include "StdAfx.h"
#include "AppException.h"

//Non-argument constructor
CAppException::CAppException(void)
{
	m_strError = _T("");
	m_nCode = -1;
}

//	Overloaded constructor
CAppException::CAppException(CString strError)
{
	m_strError = strError;
	m_nCode = 0;
}

// Destructor
CAppException::~CAppException(void)
{
}


CString CAppException::GetErrorMessage()
{
	return m_strError;
}

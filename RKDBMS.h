

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


class CRKDBMSApp : public CWinApp
{
public:
	CRKDBMSApp();


// Overrides
public:
	virtual BOOL InitInstance();   // Initialize the application instance function
	virtual int ExitInstance();    // Exit the application instance function

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRKDBMSApp theApp;

#pragma once


// CDBView view
/****************************************************
[ClassName] CDBView
[Function] View class of displaying database structure, inherits from CTreeView class.
****************************************************/
class CDBView : public CTreeView
{
	DECLARE_DYNCREATE(CDBView)

protected:
	CDBView();           // protected constructor used by dynamic creation
	virtual ~CDBView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};



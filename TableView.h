#pragma once
#include "TableEntity.h"	
#include "FieldEntity.h"	

// CTableView view
/******************************************************
[ClassName]	CTableView
[Function]	Table structure view class, display table structure, inherit from CListView
******************************************************/
class CTableView : public CListView
{
	DECLARE_DYNCREATE(CTableView)

protected:
	CTableView();           
	virtual ~CTableView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	CListCtrl* m_pListCtrl;	
	CTableEntity* m_pTable;	

	// Add field
	void AddField(CFieldEntity* pField);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
};



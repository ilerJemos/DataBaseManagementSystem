#pragma once

#include "TableEntity.h"	
#include "FieldEntity.h"	

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
	virtual void OnInitialUpdate();	//	View initialization function
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

protected:
	CTreeCtrl* m_pTreeCtrl;	// Tree control

private:
	HTREEITEM AddTableNode(CTableEntity* pTable);	// Add a table item
	HTREEITEM AddFieldNode(CFieldEntity* pField, HTREEITEM hTableItem); // Add the field item
	HTREEITEM GetTableItem(CString strTableName);	// Get to table item
private:
	CImageList m_imageList;	//	Tree image list

public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
};



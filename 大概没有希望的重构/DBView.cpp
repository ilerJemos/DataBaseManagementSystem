// DBView.cpp : implementation file
//

#include "stdafx.h"
#include "DBMS.h"
#include "DBView.h"

#include "DBMSDoc.h"

// CDBView

IMPLEMENT_DYNCREATE(CDBView, CTreeView)

CDBView::CDBView()
{

}

CDBView::~CDBView()
{
}

BEGIN_MESSAGE_MAP(CDBView, CTreeView)
END_MESSAGE_MAP()


// CDBView diagnostics

#ifdef _DEBUG
void CDBView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CDBView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDBView message handlers

/***********************************************
[FunctionName]	OnInitialUpdate
[Function]	View initialization function, initialize tree view
[Argument]	void
[ReturnedValue]	void
***********************************************/
void CDBView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	//	Get the document object
	CDBMSDoc* pDoc = (CDBMSDoc*)this->GetDocument();

	//	Get the exceptionl information
	CString strError = pDoc->GetError();

	//	Decide whether there are exceptions
	if (strError.GetLength() != 0)
	{
		//	Prompt exception information
		AfxMessageBox(strError);

		//	Set the exception information to empty
		pDoc->SetError(_T(""));
		return;
	}

	//	Delete images from the list
	m_imageList.DeleteImageList();

	// Create icon list
	m_imageList.Create(16, 16, ILC_COLOR16 | ILC_MASK, 0, 4); 

	// Add images to the list
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DATABASE));

	//	Get tree control
	CTreeCtrl &treeCtrl = this->GetTreeCtrl();

	// Get the style of the tree control
	treeCtrl.SetImageList(&m_imageList, TVSIL_NORMAL);

	//	Get the database name
	CString strDBName = pDoc->GetDBEntity().GetName();

	//	Add root item
	HTREEITEM hRoot = treeCtrl.InsertItem(strDBName, 0, 0, NULL);

	//	Expand item
	treeCtrl.Expand(hRoot, TVE_EXPAND);
}
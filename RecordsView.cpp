// RecordsView.cpp : implementation file
//

#include "stdafx.h"
#include "RKDBMS.h"
#include "RecordsView.h"

#include "TableEntity.h"
#include "FieldEntity.h"
#include "Global.h"
#include "RKDBMSDoc.h"


// CRecordsView

IMPLEMENT_DYNCREATE(CRecordsView, CListView)

CRecordsView::CRecordsView()
{
	m_pListCtrl = NULL;
	m_pTable = NULL;
}

CRecordsView::~CRecordsView()
{
}

BEGIN_MESSAGE_MAP(CRecordsView, CListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CRecordsView::OnNMRClick)
END_MESSAGE_MAP()


// CRecordsView diagnostics

#ifdef _DEBUG
void CRecordsView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CRecordsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRecordsView message handlers


void CRecordsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// Get list control
	m_pListCtrl = &this->GetListCtrl();

	// Get the default style
	DWORD dwStyle1= ::GetWindowLong(m_pListCtrl->m_hWnd ,GWL_STYLE);

	// Set the style
	dwStyle1 |= LVS_REPORT;	// Report style
	::SetWindowLong (m_pListCtrl->m_hWnd ,GWL_STYLE, dwStyle1);

	// Set the extension style
	m_pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
}


void CRecordsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (pSender == NULL)
	{
		CRKDBMSDoc* pDoc = (CRKDBMSDoc*)this->GetDocument();
		switch (lHint)
		{
		case UPDATE_OPEN_TABLE:	// Select record
			{
				m_pTable = (CTableEntity*)pHint;

				// Display field
				int nFieldNum = m_pTable->GetFieldNum();
				for (int i = 0; i < nFieldNum; i++)
				{
					CFieldEntity* pField = m_pTable->GetFieldAt(i);
					m_pListCtrl->InsertColumn(i, pField->GetName(), LVCFMT_LEFT, 150);
				}

				// Empty list control
				m_pListCtrl->DeleteAllItems();

				// Get the number of record
				int nRdNum = pDoc->GetRecordNum();
				
				// Display the record
				for (int i = 0; i < nRdNum; i++)
				{
					AddRecord(pDoc->GetRecord(i));
				}
			}
			break;
		case UPDATE_INSERT_RECORD:	// Insert record
			{
				if (m_pTable == pDoc->GetEditTable())
				{
					CRecordEntity* pRecord = (CRecordEntity*)pHint;
					AddRecord(pRecord);
				}
			}
			break;
		default:
			break;
		}
	}
}


void CRecordsView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// Display field menu
	CPoint point;
	GetCursorPos(&point);

	// Load the menu resource to the CMenu object
	CMenu* pMenu = this->GetParentFrame()->GetMenu()->GetSubMenu(MENU_RECORD);
	// Display the menu in the position of the cursor clicked
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, AfxGetMainWnd());	

	*pResult = 0;
}

void CRecordsView::AddRecord(CRecordEntity* pRecord)
{
	// Get the number of the field
	int nCount = m_pTable->GetFieldNum();

	// Get the number of the existed record
	int nItemCount = m_pListCtrl->GetItemCount();

	if (nCount > 0)
	{
		// Insert the record into the list control
		for (int i = 0; i < nCount; i++)
		{
			CFieldEntity* pField = m_pTable->GetFieldAt(i);
			CString strValue = pRecord->Get(pField->GetName());

			if (i == 0)
			{
				m_pListCtrl->InsertItem(nItemCount, strValue);
			}
			else
			{
				m_pListCtrl->SetItemText(nItemCount, i, strValue);
			}
		}
	}
}

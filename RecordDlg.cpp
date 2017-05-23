// RecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RKDBMS.h"
#include "RecordDlg.h"
#include "afxdialogex.h"

#include "TimeHelper.h"


// CRecordDlg dialog

IMPLEMENT_DYNAMIC(CRecordDlg, CDialogEx)

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecordDlg::IDD, pParent)
{
	m_pTableEntity = NULL;
}

CRecordDlg::~CRecordDlg()
{
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_editValue);
	DDX_Control(pDX, IDC_LIST_DATA, m_lstData);
}


BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRecordDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DATA, &CRecordDlg::OnClickListData)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE, &CRecordDlg::OnKillfocusEditValue)
END_MESSAGE_MAP()


// CRecordDlg message handlers

void CRecordDlg::OnBnClickedOk()
{
	int nCount = m_lstData.GetItemCount();

	CString strField;
	CString strValue;
	// Get the value of each row in the list
	for (int i = 0; i < nCount; i++)
	{
		// Field name
		strField = m_lstData.GetItemText(i, 0);

		// Value
		strValue = m_lstData.GetItemText(i, 2);

		m_recordEntity.Put(strField, strValue);
	}

	CDialogEx::OnOK();
}


void CRecordDlg::SetTable(CTableEntity* pTable)
{
	m_pTableEntity = pTable;
}


BOOL CRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the style of report
	DWORD dwStyle= ::GetWindowLong(m_lstData.m_hWnd ,GWL_STYLE);
	dwStyle |= LVS_REPORT;// Set style
	::SetWindowLong (m_lstData.m_hWnd ,GWL_STYLE, dwStyle);
	m_lstData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	// List header
	m_lstData.InsertColumn(0, _T("Field Name"), LVCFMT_LEFT, 100);
	m_lstData.InsertColumn(1, _T("Field Type"), LVCFMT_LEFT, 100);
	m_lstData.InsertColumn(3, _T("Value"), LVCFMT_LEFT, 150);

	// Load the table information
	if (m_pTableEntity != NULL)
	{
		// Get the numbet of field
		int nFieldNum = m_pTableEntity->GetFieldNum();
		for (int i = 0; i < nFieldNum; i++)
		{
			// Get field
			CFieldEntity* pField = m_pTableEntity->GetFieldAt(i);

			// Insert field name
			int nItem = m_lstData.InsertItem(i, pField->GetName());

			// Get the field type 
			int nDataType = pField->GetDataType();
			CString strTypeName = pField->GetTypeName(nDataType);
			// Insert the field type
			m_lstData.SetItemText(i, 1, strTypeName);
			m_lstData.SetItemData(i, nDataType);
		}
	}

	// Hide the edit control
	m_editValue.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CRecordDlg::OnClickListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_dwItem = pNMListView->iItem;  // The selected row
	m_nSubItem = pNMListView->iSubItem;  // The selected column
	DWORD dwCount = m_lstData.GetItemCount();  // The number of the data item in the list

	// If the selected line number greater than 0, the column number greater than 1,
	// and no more than the number of data items in the list, respond to the events
	if (m_dwItem >= 0 && m_nSubItem == 2 && m_dwItem <= dwCount)
	{
		CFieldEntity* pField = m_pTableEntity->GetFieldAt(m_dwItem);

		// Get the size of the selected cell
		CRect rtEdit, rtClient;
		m_lstData.GetWindowRect(rtClient);
		ScreenToClient(rtClient);
		m_lstData.GetSubItemRect(m_dwItem, m_nSubItem, LVIR_LABEL, rtEdit);
		rtEdit.MoveToX(rtEdit.left + rtClient.left + 2);
		rtEdit.MoveToY(rtEdit.top + rtClient.top);
		rtEdit.bottom += 4;

		// Get the content of the selected cells
		CString strItemText = m_lstData.GetItemText(m_dwItem, m_nSubItem);
		int nLength = strItemText.GetLength();
		// Set the content of the edit control
		m_editValue.SetWindowText(strItemText);
		m_editValue.SetWindowPos(&wndTop, rtEdit.left, rtEdit.top, rtEdit.Width(), rtEdit.Height(), SWP_SHOWWINDOW);
		m_editValue.SetSel(nLength, nLength);
		m_editValue.SetFocus();
	}
}


void CRecordDlg::OnKillfocusEditValue()
{
	// Get the value of the edit control
	CString strValue;
	m_editValue.GetWindowText(strValue);

	// Assign the value of the edit control to list item
	m_lstData.SetItemText(m_dwItem, m_nSubItem, strValue);

	// Hide the edit control
	m_editValue.ShowWindow(SW_HIDE);
}


CRecordEntity CRecordDlg::GetRecord(void)
{
	return m_recordEntity;
}

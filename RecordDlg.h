#pragma once


// CRecordDlg dialog
#include "TableEntity.h"	
#include "RecordEntity.h"	


class CRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRecordDlg();

public:
	void SetTable(CTableEntity* pTable);	// Set the value of the attribute
// Dialog Data
	enum { IDD = IDD_RECORD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();	
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editValue;		// Object of text control
	CListCtrl m_lstData;	// Object of list control

private:
	CTableEntity* m_pTableEntity;	// Pointer to the object of table entity
	DWORD m_dwItem;		// Row
	int m_nSubItem;		// Column
	CRecordEntity m_recordEntity;	// Record the object of view

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillfocusEditValue();
	CRecordEntity GetRecord(void);
};

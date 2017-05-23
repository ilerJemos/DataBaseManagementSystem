
// MainFrm.h : interface of the CMainFrame class
//

#pragma once


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;     // The toolbar object
	CStatusBar        m_wndStatusBar;   // The status bar object
	CSplitterWnd      m_wndSpliter;     // Separation of the window
	BOOL	m_bOpenDatabase;			// Whether has opened database

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);  // Function of creating frame window 
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnNewTable();
	afx_msg void OnAddField();
	// Switch the view
	void SwitchView(int nViewType);
	afx_msg void OnOpenDatabase();
	afx_msg void OnUpdateOpenDatabase(CCmdUI *pCmdUI);
	afx_msg void OnEditTable();
	afx_msg void OnOpenTable();
	afx_msg void OnInsertRecord();
};



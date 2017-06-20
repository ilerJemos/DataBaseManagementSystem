
// DBMSDoc.cpp : CDBMSDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DBMS.h"
#endif

#include "DBMSDoc.h"

#include "AppException.h"

#include "DBEntity.h"	
#include "DBLogic.h"	

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDBMSDoc

IMPLEMENT_DYNCREATE(CDBMSDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBMSDoc, CDocument)
END_MESSAGE_MAP()


// CDBMSDoc 构造/析构

CDBMSDoc::CDBMSDoc()
{
	m_strError = _T("");	//	Initialize the exception information

}

CDBMSDoc::~CDBMSDoc()
{
}

/***************************************************************
[FunctionName] OnNewDocument
[Function] Create a new document
[Argument] void
[ReturnedValue] BOOL:TRUE if the operation is successful; otherwise FALSE.
***************************************************************/
BOOL CDBMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_dbEntity.SetName(_T("Test"));	// The default database is Ruanko

	CDBLogic dbLogic;
	try
	{
		//	Decide whether exists the database
		if (dbLogic.GetDatabase(m_dbEntity) == false)
		{
			//	Decide whether creates the database successfully
			if (dbLogic.CreateDatabase(m_dbEntity) == false)
			{
				//	If creates failure, throw an exception
				throw new CAppException(_T("Failed to create database！"));
			}

		}

		// Set the document title bar
		CString strTitle;
		strTitle.Format(_T("Database(%s)"), m_dbEntity.GetName());
		this->SetTitle(strTitle);
	}
	catch (CAppException* e)	// Catch custom exception
	{
		// Throw exception
		m_strError = e->GetErrorMessage();
		delete e;
	}

	return TRUE;
}




// CDBMSDoc 序列化

void CDBMSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDBMSDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CDBMSDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CDBMSDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDBMSDoc 诊断

#ifdef _DEBUG
void CDBMSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDBMSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDBMSDoc 命令

/*****************************************************
[FunctionName] GetDBEntity
[Function]	Get the database entity
[Argument]	void
[ReturnedValue] CDBEntity: Database entity
*****************************************************/
CDBEntity CDBMSDoc::GetDBEntity()
{
	return m_dbEntity;
}

/*****************************************************
[FunctionName] GetError
[Function]	Get exception information
[Argument]	void
[ReturnedValue] CString: Exception information string
*****************************************************/
CString CDBMSDoc::GetError()
{
	//	Remove white space characters
	m_strError.TrimLeft();
	m_strError.TrimRight();

	return m_strError;
}

/*****************************************************
[FunctionName] SetError
[Function]	Set exception information
[Argument]	CString strError: Exception information string
[ReturnedValue] void
*****************************************************/
void CDBMSDoc::SetError(CString strError)
{
	m_strError = strError;
}
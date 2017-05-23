
// RKDBMSDoc.cpp : implementation of the CRKDBMSDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RKDBMS.h"
#endif

#include "RKDBMSDoc.h"

#include "DBLogic.h"		
#include "TableLogic.h"		
#include "RecordLogic.h"	

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRKDBMSDoc

IMPLEMENT_DYNCREATE(CRKDBMSDoc, CDocument)

BEGIN_MESSAGE_MAP(CRKDBMSDoc, CDocument)
END_MESSAGE_MAP()


// CRKDBMSDoc construction/destruction

CRKDBMSDoc::CRKDBMSDoc()
{
	m_strError = _T("");	//	Initialize the exception information
	m_pEditTable = NULL;

}

CRKDBMSDoc::~CRKDBMSDoc()
{
}


BOOL CRKDBMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_dbEntity.SetName(_T("Ruanko"));	

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
				throw new CAppException(_T("Failed to create database£¡"));
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

// CRKDBMSDoc serialization

void CRKDBMSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CRKDBMSDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
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

#endif // SHARED_HANDLERS

// CRKDBMSDoc diagnostics

#ifdef _DEBUG
void CRKDBMSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRKDBMSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG




CDBEntity CRKDBMSDoc::GetDBEntity()
{
	return m_dbEntity;
}


CString CRKDBMSDoc::GetError()
{
	//	Remove white space characters
	m_strError.TrimLeft();
	m_strError.TrimRight();

	return m_strError;
}


void CRKDBMSDoc::SetError(CString strError)
{
	m_strError = strError;
}


CTableEntity* CRKDBMSDoc::CreateTable(CString strName)
{
	CTableEntity* pTable = new CTableEntity();
	pTable->SetName(strName);
	CTableLogic tbLogic;
	try
	{
		// Decide whether creates table successfully
		if (tbLogic.CreateTable(m_dbEntity.GetName(), *pTable) == true)
		{
			// If creates table successfully, the created table information would be saved to the array.
			m_arrTable.Add(pTable);
		}
		else
		{
			// If creates failure, release the memory allocated by new
			delete pTable;
			pTable = NULL;
		}
	}
	catch(CAppException* e)	// Catch exception
	{
		// If there is exception, release the memory allocated by new
		if (pTable != NULL)
		{
			delete pTable;
			pTable = NULL;
		}
		// Get exception information
		m_strError = e->GetErrorMessage();
		// Delete exception
		delete e;
	}

	return pTable;
}


void CRKDBMSDoc::OnCloseDocument()
{
	// Get the number of elements in the table array.
	int nCount = m_arrTable.GetCount();

	// Traverse the table array
	for (int i = 0; i < nCount; i++)
	{
		// Release the elements in the array
		CTableEntity* pTable = (CTableEntity*)m_arrTable.GetAt(i);
		if (pTable != NULL)
		{
			delete pTable;
			pTable = NULL;
		}
	}
	// Empty table array
	m_arrTable.RemoveAll();

	// Get the number of elements in the record array.
	nCount = m_arrRecord.GetCount();
	for (int j = 0; j < nCount; j++)
	{
		CRecordEntity* pRecord = (CRecordEntity*)m_arrRecord.GetAt(j);
		if (pRecord != NULL)
		{
			delete pRecord;
			pRecord = NULL;
		}
	}
	// Empty record array
	m_arrRecord.RemoveAll();

	CDocument::OnCloseDocument();
}


CTableEntity* CRKDBMSDoc::GetEditTable()
{
	return m_pEditTable;
}


CFieldEntity* CRKDBMSDoc::AddField(CFieldEntity &field)
{
	CTableLogic tbLogic;
	try
	{
		// Add a field
		if(tbLogic.AddField(m_dbEntity.GetName(), *m_pEditTable, field) == false)
		{
			return NULL;
		}

		// Get field number
		int nCount = m_pEditTable->GetFieldNum();

		// Return the last field
		CFieldEntity* pField = m_pEditTable->GetFieldAt(nCount -1);
		return pField;
	}
	catch(CAppException* e)
	{
		m_strError = e->GetErrorMessage();
		delete e;
		return NULL;
	}
}


void CRKDBMSDoc::SetEditTable(CString strTableName)
{
	if (strTableName.GetLength() > 0)
	{
		// Get the number of the table
		int nCount = m_arrTable.GetCount();

		// Find the table information of the same table name
		for (int i = 0; i  < nCount; i++)
		{
			CTableEntity* pTable = m_arrTable.GetAt(i);
			if (pTable->GetName().CollateNoCase(strTableName) == 0)
			{
				m_pEditTable = pTable;
				CString strTitle;
				strTitle.Format(_T("Form(%s)"), m_pEditTable->GetName());
				this->SetTitle(strTitle);
				return;
			}
		}
	}
	else
	{
		m_pEditTable = NULL;
		CString strTitle;
		strTitle.Format(_T("Database(%s)"), m_dbEntity.GetName());
		this->SetTitle(strTitle);
	}	
}


void CRKDBMSDoc::LoadTables(void)
{
	CTableLogic tbLogic;

	try
	{
		// Get the number of table in the table array
		int nCount = m_arrTable.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			// Release the elements in the array
			CTableEntity* pTable = (CTableEntity*)m_arrTable.GetAt(i);
			if (pTable != NULL)
			{
				delete pTable;
				pTable = NULL;
			}
		}

		// Empty array
		m_arrTable.RemoveAll();

		// Get table information
		tbLogic.GetTables(m_dbEntity.GetName(), m_arrTable);
	}
	catch(CAppException* e)
	{
		m_strError = e->GetErrorMessage();
		delete e;
	}
}


CTableEntity* CRKDBMSDoc::GetTable(int nIndex)
{
	return (CTableEntity*)m_arrTable.GetAt(nIndex);
}


int CRKDBMSDoc::GetTableNum()
{
	return m_arrTable.GetCount();
}


CRecordEntity* CRKDBMSDoc::InsertRecord(CRecordEntity &record)
{		
	CRecordLogic recordLogic;
	CRecordEntity* pRecord = new CRecordEntity(record);
	try
	{
		// Insert record
		if (recordLogic.Insert(m_dbEntity.GetName(), *m_pEditTable, record) == true)
		{
			// Save the record which is added successfully to the record array.
			m_arrRecord.Add(pRecord);
			return pRecord;
		}
	}
	catch (CAppException* e)
	{
		if (pRecord != NULL)
		{
			delete pRecord;
			pRecord = NULL;
		}
		m_strError = e->GetErrorMessage();
		delete e;		
	}

	return NULL;
}


int CRKDBMSDoc::GetRecordNum()
{
	return m_arrRecord.GetCount();
}


CRecordEntity* CRKDBMSDoc::GetRecord(int nIndex)
{
	return (CRecordEntity*)m_arrRecord.GetAt(nIndex);
}


void CRKDBMSDoc::LoadRecord(void)
{
	// Get the number of table in the table array
	int nCount = m_arrRecord.GetCount();

	// Release the each element in the array.
	for(int i = 0; i < nCount; i++)
	{
		CRecordEntity* pRecord = (CRecordEntity*)m_arrRecord.GetAt(i);

		delete pRecord;
		pRecord = NULL;
	}

	// Empty array
	m_arrRecord.RemoveAll();

	try
	{
		CRecordLogic recordLogic;
		// Decide whether get record successfully
		if (recordLogic.SelectAll(*m_pEditTable, m_arrRecord) == false)
		{
			// If failed, decide the existing records
			nCount = m_arrRecord.GetCount();

			// If exists record, empty it
			if(nCount > 0)
			{
				for(int i = 0; i < nCount; i++)
				{
					CRecordEntity* pRecord = (CRecordEntity*)m_arrRecord.GetAt(i);

					delete pRecord;
					pRecord = NULL;
				}

				m_arrRecord.RemoveAll();
			}
		}
	}
	catch (CAppException* e)
	{
		// If there is exception, save exception information and delete the exception object
		m_strError = e->GetErrorMessage();
		delete e;
	}
}
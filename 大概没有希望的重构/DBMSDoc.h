#include"DBEntity.h"

// DBMSDoc.h : CDBMSDoc ��Ľӿ�
//


#pragma once

/*****************************************************
[ClassName] CRKDBMSDoc
[Function] Document class, implementation procedures related to the 
           interface of logic operation, and the data maintenance and management.
*****************************************************/
class CDBMSDoc : public CDocument
{
protected: // �������л�����
	CDBMSDoc();
	DECLARE_DYNCREATE(CDBMSDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	// virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CDBMSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CDBEntity m_dbEntity;	// Database entity object
	CString m_strError;		// Exception information

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	// void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	CDBEntity GetDBEntity();
	CString GetError();
	void SetError(CString strError);
};

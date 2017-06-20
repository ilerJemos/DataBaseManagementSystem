#include"DBEntity.h"

// DBMSDoc.h : CDBMSDoc 类的接口
//


#pragma once

/*****************************************************
[ClassName] CRKDBMSDoc
[Function] Document class, implementation procedures related to the 
           interface of logic operation, and the data maintenance and management.
*****************************************************/
class CDBMSDoc : public CDocument
{
protected: // 仅从序列化创建
	CDBMSDoc();
	DECLARE_DYNCREATE(CDBMSDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	// virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CDBMSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CDBEntity m_dbEntity;	// Database entity object
	CString m_strError;		// Exception information

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	// void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	CDBEntity GetDBEntity();
	CString GetError();
	void SetError(CString strError);
};

// CMSDoc.cpp : CCMSDoc 类的实现
//

#include "stdafx.h"
#include "CMS.h"

#include "CMSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCMSDoc

IMPLEMENT_DYNCREATE(CCMSDoc, CDocument)

BEGIN_MESSAGE_MAP(CCMSDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CCMSDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CCMSDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()


// CCMSDoc 构造/析构

CCMSDoc::CCMSDoc()
{
	// TODO: 在此添加一次性构造代码

}

CCMSDoc::~CCMSDoc()
{
}

BOOL CCMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CCMSDoc 序列化

void CCMSDoc::Serialize(CArchive& ar)
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


// CCMSDoc 诊断

#ifdef _DEBUG
void CCMSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCMSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCMSDoc 命令

// CMSDoc.cpp : CCMSDoc ���ʵ��
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


// CCMSDoc ����/����

CCMSDoc::CCMSDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CCMSDoc::~CCMSDoc()
{
}

BOOL CCMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CCMSDoc ���л�

void CCMSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CCMSDoc ���

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


// CCMSDoc ����

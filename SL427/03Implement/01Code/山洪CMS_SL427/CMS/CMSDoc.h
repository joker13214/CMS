// CMSDoc.h : CCMSDoc ��Ľӿ�
//


#pragma once


class CCMSDoc : public CDocument
{
protected: // �������л�����
	CCMSDoc();
	DECLARE_DYNCREATE(CCMSDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CCMSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};



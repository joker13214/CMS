// UdfSplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfSplitterWnd.h"


// CUdfSplitterWnd

IMPLEMENT_DYNAMIC(CUdfSplitterWnd, CSplitterWnd)

CUdfSplitterWnd::CUdfSplitterWnd()
{
	m_cxSplitter = 4;
	m_cySplitter = 4;
	m_cxBorderShare =0;
	m_cyBorderShare = 0;
	m_cxSplitterGap = 2;
	m_cySplitterGap =2;
}

CUdfSplitterWnd::~CUdfSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CUdfSplitterWnd, CSplitterWnd)
END_MESSAGE_MAP()



// CUdfSplitterWnd 消息处理程序



#pragma once


// CDataInDbFrame 框架
// 数据信息实时入库窗口
class CDataInDbFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CDataInDbFrame)
protected:
	CDataInDbFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CDataInDbFrame();

protected:
	DECLARE_MESSAGE_MAP()
};



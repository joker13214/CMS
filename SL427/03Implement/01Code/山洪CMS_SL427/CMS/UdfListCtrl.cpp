// UdfListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "UdfListCtrl.h"
#include "UdfLogSystem.h"


// CUdfListCtrl

IMPLEMENT_DYNAMIC(CUdfListCtrl, CListCtrl)

CUdfListCtrl::CUdfListCtrl()
{

}

CUdfListCtrl::~CUdfListCtrl()
{
}


BEGIN_MESSAGE_MAP(CUdfListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CUdfListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()



// CUdfListCtrl 消息处理程序



void CUdfListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	COLORREF clr ;
	COLORREF fClr;
	clr = RGB(255,255,255);
	fClr = RGB(0,0,0);

	switch(pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:

		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		{
			int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
			_Log_level_type dValue = (_Log_level_type)GetItemData(nItem);


			switch(dValue)
			{
			case Normal_Log_Message:
				clr = RGB(255,255,255);
				break;
			case Warning_log_Message:
				clr = RGB(255,255,0);
				break;
			case Error_Log_message:
				{
					clr =RGB(255,0,0);
					fClr =RGB(255,255,255);
				}
				
				break;
			}

			pLVCD->clrTextBk = clr;
			pLVCD->clrText = fClr;
		}
		break;
	}
}

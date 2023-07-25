#pragma once


// CUdfListCtrl

class CUdfListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CUdfListCtrl)

public:
	CUdfListCtrl();
	virtual ~CUdfListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};



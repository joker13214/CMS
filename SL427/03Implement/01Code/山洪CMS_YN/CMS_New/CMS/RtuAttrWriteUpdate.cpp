// RtuAttrWriteUpdate.cpp : 实现文件
//

#include "stdafx.h"
#include "CMS.h"
#include "RtuAttrWriteUpdate.h"


// CRtuAttrWriteUpdate 对话框
#define W_TIMER_WRITEUPDATE_SEND 0x01 //发送
#define W_TIMER_WRITEUPDATE_READ 0x02 //读取
#define W_TIMER_WRITEUPDATE_START 0x03//启动

#define W_TIMER_WRITEUPDATE_A0   0x04
#define W_TIMER_WRITEUPDATE_A1   0x05
#define W_TIMER_WRITEUPDATE_A2   0x06
#define W_TIMER_WRITEUPDATE_A3   0x07
#define W_TIMER_WRITEUPDATE_B0   0x08
#define W_TIMER_WRITEUPDATE_B1   0x09
#define W_TIMER_WRITEUPDATE_B2   0x0A
#define W_TIMER_WRITEUPDATE_B3   0x0B

IMPLEMENT_DYNAMIC(CRtuAttrWriteUpdate, CDialog)

CRtuAttrWriteUpdate::CRtuAttrWriteUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuAttrWriteUpdate::IDD, pParent)
{
	m_blResult = FALSE;
	m_pSubStation = NULL;
	m_pParentDlg = NULL;
	m_nAttType = 0xff;
	m_nRepeatCnt = 0;
	m_nRepeatFailCnt = 0;
	m_nPackCntSum = 0;
	m_IsStartBtnClicked = false;
	m_HasUpdateFile = false;
	m_bCheckBoxChecked = false;
	m_nComInterface = Ethernet_CommInerface;
	m_bIsUpdating = false;
}

CRtuAttrWriteUpdate::~CRtuAttrWriteUpdate()
{
}

void CRtuAttrWriteUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BTN_START ,m_StartBtn);
	DDX_Control(pDX,IDC_BTN_READ ,m_ReadFileBtn);	
	DDX_Control(pDX,IDC_BTN_SEND ,m_SendBtn);
	DDX_Control(pDX,IDC_CANCLE, m_CancleBtn);
	DDX_Control(pDX,IDC_UPDATE, m_CheckBox);
	DDX_Control(pDX,IDC_EDIT_FILEADDR ,m_FileAddress);
}

BOOL CRtuAttrWriteUpdate::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWndsEnable(FALSE, 1);

	CWnd* pWnd = GetDlgItem(IDC_STATIC_WATERLEVELGRID);
	CRect rect1, rect2;
	int captionHeight = 0; 
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	this->GetWindowRect(&rect2);
	if (pWnd)
		pWnd->GetWindowRect(&rect1);
	if (!m_GridCtrl.Create(CRect(rect1.left - rect2.left - cxframe, 
		rect1.top-rect2.top - cyframe-captionHeight, 
		rect1.left + rect1.Width() - rect2.left, 
		rect1.top + rect1.Height() - rect2.top - captionHeight), 
		this, IDC_WATERLEVEL_GRIDCTRL,WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to create threads  grid!\n");
		return FALSE;
	}

	m_GridCtrl.SetBkColor(RGB(255,255,255));
	m_GridCtrl.SetListMode(TRUE);
	m_GridCtrl.SetSingleColSelection(TRUE);
	m_GridCtrl.SetSingleRowSelection(TRUE);

	return TRUE;

}


BEGIN_MESSAGE_MAP(CRtuAttrWriteUpdate, CDialog)
	ON_BN_CLICKED(IDC_BTN_START, &CRtuAttrWriteUpdate::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_READ, &CRtuAttrWriteUpdate::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_SEND, &CRtuAttrWriteUpdate::OnBnClickedBtnSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_UPDATE, &CRtuAttrWriteUpdate::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_CANCLE, &CRtuAttrWriteUpdate::OnBnClickedCancle)
END_MESSAGE_MAP()

void CRtuAttrWriteUpdate::SetParentDlg(CRtuPropertyDlg* pDlg)
{
	m_pParentDlg = pDlg;
}

void CRtuAttrWriteUpdate::Update()
{
	if (m_pParentDlg)
		m_pSubStation = m_pParentDlg->GetRtuStation();	


// 	if (m_GridCtrl.GetSafeHwnd())
// 	{
// 		m_GridCtrl.DeleteAllItems();
// 		m_GridCtrl.Refresh();
// 	}

	if (m_pSubStation != NULL)
	{
// 		CHydrWaterLevel *pWaterLevel = m_pSubStation->GetHydrWaterLevel();
// 		if (NULL == pWaterLevel)
// 		{
// 			return;
// 		}
// 
// 		if (m_wndSensorNoCbx.GetSafeHwnd())
// 		{
// 			int nSel = m_wndSensorNoCbx.GetCurSel();
// 			if (CB_ERR == nSel)
// 				return;
// 
// 			CString strText = "";
// 			m_wndSensorNoCbx.GetLBText(nSel, strText);
// 
// 			if (pWaterLevel->GetSensorNo() == atoi(strText))
// 			{
// 				OnCbnSelChangeSensorNo();
// 			}
// 
// 		}
	}

}

void CRtuAttrWriteUpdate::UpdateDlg(int attrType, int btnCmd, BOOL blResult, int CommInterface)
{
	m_blResult = blResult;
	m_nComInterface = CommInterface;

	switch(btnCmd)
	{
	case WriteUpdate_Start:
		m_nAttType = attrType;
		SendMessage(WM_TIMER, W_TIMER_WRITEUPDATE_START, NULL);
		break;
	case WriteUpdate_Send:
		m_nAttType = attrType;
		SendMessage(WM_TIMER, W_TIMER_WRITEUPDATE_SEND, NULL);
	}
	
}

void CRtuAttrWriteUpdate::KillAllTimers()
{
	KillTimer(W_TIMER_WRITEUPDATE_SEND);
	KillTimer(W_TIMER_WRITEUPDATE_READ);
	KillTimer(W_TIMER_WRITEUPDATE_START);
	SetWndsEnable(TRUE, 2);
}

void CRtuAttrWriteUpdate::OnTimer(UINT_PTR nIDEvent)
{
	if (m_pSubStation)
	{
		CString strLogMessge = "";
		_Log_level_type _logLevle = Normal_Log_Message;

		bool bKillTimer = false;
		bool isCMD_B1 = false;
		switch (nIDEvent)
		{
		case W_TIMER_WRITEUPDATE_START:
			{
				if (m_blResult == TRUE)
				{
					switch(m_nAttType)
					{
					case writeUpdate_A0:
					case writeUpdate_B0:
						strLogMessge = "升级启动 成功!";
						m_nRepeatCnt = 0;
						m_nRepeatFailCnt = 0;
						SetWndsEnable(TRUE, 1);
						bKillTimer = true;
						break;
					case writeUpdate_A1:
					case writeUpdate_A2:
					case writeUpdate_A3:
					case writeUpdate_B1:
					case writeUpdate_B2:
					case writeUpdate_B3:
						{
							if (m_nRepeatCnt < 3)
							{
								strLogMessge = "升级启动 RTU回复不正确, 重新发送启动升级报文";
								OnBnClickedBtnStart();
								m_nRepeatCnt++;
							}
							else
							{
								strLogMessge = "升级启动 失败！RTU连续三次回复错误";
								SetWndsEnable(FALSE, 1);
								m_nRepeatCnt = 0;
								bKillTimer = true;
							}
						}		
						break;			
					}		
				}
				else 
				{
					if (m_nComInterface == Serial_CommInterface)
					{
						if (m_nRepeatFailCnt < 6)
						{
							strLogMessge = "发送 超时, 进行重发";
							_logLevle = Warning_log_Message;
							OnBnClickedBtnStart();
							m_nRepeatFailCnt++;

						}
						else
						{
							strLogMessge = "升级启动 失败!";
							SetWndsEnable(FALSE, 1);
							_logLevle = Warning_log_Message;
							bKillTimer = true;
						}		
					}
					else
					{
						if (m_nRepeatFailCnt < 1)
						{
							strLogMessge = "发送 超时, 进行重发";
							_logLevle = Warning_log_Message;
							OnBnClickedBtnStart();
							m_nRepeatFailCnt++;

						}
						else
						{
							strLogMessge = "升级启动 失败!";
							SetWndsEnable(FALSE, 1);
							_logLevle = Warning_log_Message;
							bKillTimer = true;
						}		
					}
					
				}
			}
			break;
		case W_TIMER_WRITEUPDATE_SEND:
			{
				
				if (m_blResult == TRUE)
				{
					if (!m_IsStartBtnClicked)
					{
						strLogMessge = "请先启动升级程序";
						break;
					}

					switch(m_nAttType)
					{
					case writeUpdate_A0:
						{
							if (m_pSubStation->bIteratorVaild)
							{
								strLogMessge = "发送 成功, 升级未完成，请等待!";
								m_pSubStation->m_it_map++;
								if (m_pSubStation->m_it_map != m_pSubStation->m_WriteUpdateMap.end())
								{
									int n = m_pSubStation->m_it_map->first;
									m_nRepeatCnt = 0;
									m_nRepeatFailCnt = 0;
									OnBnClickedBtnSend();
								}
								else
								{
									m_pSubStation->m_it_map = m_pSubStation->m_WriteUpdateMap.begin();
									if (m_pSubStation->m_it_map != m_pSubStation->m_WriteUpdateMap.end())
									{
										m_nRepeatCnt = 0;
										m_nRepeatFailCnt = 0;
										OnBnClickedBtnSend();
									}
									else
									{
										strLogMessge = "请重新导入升级文件进行升级!";

									}

								}
								
							}
							else
							{
								strLogMessge = "升级命令A0，出现无效迭代器";
								bKillTimer = true;

							}
							
						}
						break;
					case writeUpdate_A1:
						{
							if (m_pSubStation->bIteratorVaild)
							{
								if (m_nRepeatCnt < 3)
								{
									strLogMessge = "跳包发送，重发前一条!";
									m_pSubStation->m_it_map++;
									OnBnClickedBtnSend();
									m_nRepeatCnt++;
								}
								else
								{
									strLogMessge = "升级 失败！RTU连续三次回复错误";
									SetWndsEnable(FALSE, 1);
									m_nRepeatCnt = 0;
									m_nRepeatFailCnt = 0;
									bKillTimer = true;
								}
							}
							else
							{
								strLogMessge = "升级命令A1，出现无效迭代器";

							}
						
							
						}
						break;
					case writeUpdate_A2:
						{
							if (m_pSubStation->bIteratorVaild)
							{
								if (m_nRepeatCnt < 3)
								{
									strLogMessge = "发送序号相同，内容不同，重发当前条!";
									OnBnClickedBtnSend();
									m_nRepeatCnt++;
								}
								else
								{
									strLogMessge = "升级 失败！RTU连续三次回复错误";
									SetWndsEnable(FALSE, 1);
									m_nRepeatCnt = 0;
									m_nRepeatFailCnt = 0;
									bKillTimer = true;
								}	
							}
							else
							{
								strLogMessge = "升级命令A2，出现无效迭代器";
								bKillTimer = true;
							}	
						}
						break;
					case writeUpdate_A3:
						{
							if (m_pSubStation->bIteratorVaild)
							{
								if (m_nRepeatCnt < 3)
								{
									strLogMessge = "发送序号相同，内容不同，重发当前条!";
									OnBnClickedBtnSend();
									m_nRepeatCnt++;
								}
								else
								{
									strLogMessge = "升级 失败！RTU连续三次回复错误";
									SetWndsEnable(FALSE, 1);
									m_nRepeatCnt = 0;
									m_nRepeatFailCnt = 0;
									bKillTimer = true;
								}	
							}
							else
							{
								strLogMessge = "升级命令A3，出现无效迭代器";
								bKillTimer = true;

							}
							
						}
						break;
					case writeUpdate_B1:
						{
							if (m_pSubStation->bIteratorVaild)
							{
								strLogMessge = "接收并验证正确，升级报文全部接收成功";
								//OnBnClickedBtnSend();
								SetWndsEnable(FALSE, 1);
								bKillTimer = true;
								isCMD_B1 = true;

							}
							else
							{
								strLogMessge = "升级命令B1，出现无效迭代器";
								bKillTimer = true;//add
							}		
						}
						break;
					case writeUpdate_B2:
						{
							if (m_pSubStation->bIteratorVaild)
							{
								strLogMessge = "程序升级成功";
								SetWndsEnable(FALSE, 1);
								bKillTimer = true;
							}
							else
							{
								strLogMessge = "升级命令B2，出现无效迭代器";
								bKillTimer = true;
							}		
						}
						break;
					case writeUpdate_B3:
						{
							if (m_pSubStation->bIteratorVaild)
							{
								strLogMessge = "连续升级三次，升级失败！";
								SetWndsEnable(FALSE, 1);
								bKillTimer = true;
							}
							else
							{
								strLogMessge = "升级命令B2，出现无效迭代器";
								bKillTimer = true;
							}

						}
						break;

					}
				}
				else 
				{
					if (m_nRepeatFailCnt < 1)
					{
						strLogMessge = "发送 超时, 进行重发";
						_logLevle = Warning_log_Message;
						OnBnClickedBtnSend();
						m_nRepeatFailCnt++;
					}
					else
					{
						strLogMessge = "发送 失败, 重新启动升级";
						m_pSubStation->curr_it = m_pSubStation->m_pWriteUpdateList.begin();
						_logLevle = Warning_log_Message;
						SetWndsEnable(FALSE, 1);
						bKillTimer = true;
						m_nRepeatFailCnt = 0;

					}
					
				}

			}
		}

		if (bKillTimer)
		{
			KillTimer(nIDEvent);
		}

		if (isCMD_B1)
		{
			UINT nElapse = 20 * 1000;
			SetTimer(W_TIMER_WRITEUPDATE_SEND, nElapse, NULL);
		}
		
		if (m_pParentDlg && m_pParentDlg->GetSafeHwnd())
			m_pParentDlg->AddLog(strLogMessge, NULL, _logLevle);

		if (m_pSubStation)
			m_pSubStation->InsertLogMessage(strLogMessge, _logLevle, NULL);
	}
	

	CDialog::OnTimer(nIDEvent);
}

void CRtuAttrWriteUpdate::SetWndsEnable(BOOL blEnable, int ret)
{
	switch (ret)
	{
	case 1:
		m_StartBtn.EnableWindow(!blEnable);
		m_ReadFileBtn.EnableWindow(blEnable);
		m_SendBtn.EnableWindow(blEnable);
		m_FileAddress.EnableWindow(blEnable);
		m_CancleBtn.EnableWindow(blEnable);
		m_CheckBox.EnableWindow(blEnable);
		break;
	case 2:
		m_StartBtn.EnableWindow(blEnable);
		m_ReadFileBtn.EnableWindow(blEnable);
		m_SendBtn.EnableWindow(blEnable);
		m_FileAddress.EnableWindow(blEnable);
		m_CancleBtn.EnableWindow(blEnable);
		m_CheckBox.EnableWindow(blEnable);
		break;
	case 3:
		m_StartBtn.EnableWindow(blEnable);
		m_ReadFileBtn.EnableWindow(blEnable);
		m_SendBtn.EnableWindow(blEnable);
		m_FileAddress.EnableWindow(blEnable);
		break;
	}

}

void CRtuAttrWriteUpdate::InitGridItems()
{
	int nSel = 0;
	if (m_GridCtrl.GetSafeHwnd())
	{
		m_GridCtrl.DeleteAllItems();
		if (!m_pSubStation) return;

		CString strHeader[] = {
			"已发送",
			"装置地址",
			"命令码",
			"包计数",
			"代码地址",
			"代码长度",
			"代码内容"
		};

		if (m_pSubStation)
		{
			int nRow = m_pSubStation->m_pWriteUpdateList.size();
			//m_pSubStation->curr_it = m_pSubStation->m_pWriteUpdateList.begin();
			m_pSubStation->m_it_map = m_pSubStation->m_WriteUpdateMap.find(0);
			if (m_pSubStation->m_it_map != m_pSubStation->m_WriteUpdateMap.end())
			{
				m_pSubStation->bIteratorVaild = true;
			}
			else
			{
				m_pSubStation->bIteratorVaild = false;
			}

			m_GridCtrl.SetRowCount(nRow+1);
			m_GridCtrl.SetColumnCount(7);
			m_GridCtrl.SetColumnWidth(0,60);
			m_GridCtrl.SetColumnWidth(1,100);
			m_GridCtrl.SetColumnWidth(2,80);
			m_GridCtrl.SetColumnWidth(3,80);
			m_GridCtrl.SetColumnWidth(4,80);
			m_GridCtrl.SetColumnWidth(5,80);
			m_GridCtrl.SetColumnWidth(6,300);

			for (int i=0; i<7; i++)
			{
				m_GridCtrl.SetItemText(0, i, strHeader[i]);
				CUdfGlobalFUNC::SetGridCellReadOnly(&m_GridCtrl,0,i);
				m_GridCtrl.SetItemBkColour(0,i,RGB(128,128,128));
			}

			std::list<WriteDataFromFile *>::iterator it;
			it = m_pSubStation->m_pWriteUpdateList.begin();

			int nRowIdx = 1;
			WriteDataFromFile *pData = NULL;
			CString strCodeLen = "";
			CString strContext = "";
			CString strPackNo = "";
			for (it=m_pSubStation->m_pWriteUpdateList.begin();
				it!=m_pSubStation->m_pWriteUpdateList.end();
				it++)
			{
				pData = (WriteDataFromFile*)(*it);
				if (pData == NULL)
				{
					nRowIdx++;
					continue;
				}
				m_GridCtrl.SetItemText(nRowIdx, 0, _T("否"));
				m_GridCtrl.SetItemText(nRowIdx, 1, m_pSubStation->m_strStCode);
				m_GridCtrl.SetItemText(nRowIdx, 2, _T("A0"));

				strPackNo.Format("%d", pData->m_nPackNo);
				m_GridCtrl.SetItemText(nRowIdx, 3, strPackNo);
				m_GridCtrl.SetItemText(nRowIdx, 4, pData->m_strCodeAddr);
				strCodeLen.Format(_T("%d"), pData->m_nBufLen);
				m_GridCtrl.SetItemText(nRowIdx, 5, strCodeLen);
				strContext = CUdfGlobalFUNC::ConvertBytesToString(pData->m_CodeContext, pData->m_nBufLen);
				m_GridCtrl.SetItemText(nRowIdx, 6, strContext);
				nRowIdx++;
			}

			m_GridCtrl.Refresh();

		}
	}

}

// CRtuAttrWriteUpdate 消息处理程序


void CRtuAttrWriteUpdate::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pSubStation)
	{
		m_IsStartBtnClicked = true;
		m_pSubStation->bIsWriteUpdateStart = true;
		m_pSubStation->m_CurrentWriteUpdateOp = WriteUpdate_Start;
		SetWndsEnable(FALSE, 2);
		m_blResult = FALSE;

		CWriteFrame writeFrame;
		m_pSubStation->BuildBasicPacket(writeFrame);  //生成初始的下发帧报文帧数据信息

		writeFrame.Sendbuf[0] = 0x24;
		writeFrame.Sendbuf[1] = 0x24;
		writeFrame.Sendbuf[2] = 24;

		CString strStCode = writeFrame.m_nDevAdddr;
		char charStCode[11];
		memset(charStCode, 0, 10);
		memcpy(charStCode, strStCode.GetBuffer(10), 10);

		writeFrame.Sendbuf[3] = charStCode[0];
		writeFrame.Sendbuf[4] = charStCode[1];
		writeFrame.Sendbuf[5] = charStCode[2];
		writeFrame.Sendbuf[6] = charStCode[3];
		writeFrame.Sendbuf[7] = charStCode[4];
		writeFrame.Sendbuf[8] = charStCode[5];
		writeFrame.Sendbuf[9] = charStCode[6];
		writeFrame.Sendbuf[10] = charStCode[7];
		writeFrame.Sendbuf[11] = charStCode[8];
		writeFrame.Sendbuf[12] = charStCode[9];

		writeFrame.Sendbuf[13] = 0xA0;

		//包计数
		writeFrame.Sendbuf[14] = 0x00;
		writeFrame.Sendbuf[15] = 0x00;
		writeFrame.Sendbuf[16] = 0x00;
		writeFrame.Sendbuf[17] = 0x00;

		//代码地址
		writeFrame.Sendbuf[18] = 0x00;
		writeFrame.Sendbuf[19] = 0x00;
		writeFrame.Sendbuf[20] = 0x00;
		writeFrame.Sendbuf[21] = 0x00;

		//代码长度
		writeFrame.Sendbuf[22] = 0x00;
		WORD CRC = CUdfGlobalFUNC::CreateCRC_Write(writeFrame.Sendbuf, 20);
		writeFrame.Sendbuf[23] = HIBYTE(CRC);
		writeFrame.Sendbuf[24] = LOBYTE(CRC);
		writeFrame.Sendbuf[25] = 0x23;
		writeFrame.Sendbuf[26] = 0x23;


		m_pSubStation->m_pSendWriteFrame = writeFrame;
		m_pSubStation->GetSysDataTrans_WriteUpdate()->SetRtuStation(m_pSubStation);
		m_pSubStation->GetSysDataTrans_WriteUpdate()->SendRtuLinkFrame(writeFrame.Sendbuf, 27, writeFrame.m_CommInterface);
		
		UINT nElapse = 120 * 1000;
		if (Serial_CommInterface == m_pSubStation->GetInterface())
		{
			m_nComInterface = Serial_CommInterface;
			nElapse = 10 * 1000;
		}
		
		m_nComInterface = Ethernet_CommInerface;
		SetTimer(W_TIMER_WRITEUPDATE_START, nElapse, NULL);

		
	}
	else
	{
		m_IsStartBtnClicked = false;
	}

}

void CRtuAttrWriteUpdate::OnBnClickedBtnRead()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation)
	{
		using namespace std;
		typedef pair<int, WriteDataFromFile*> PAIR;

		CFileDialog fileDlg(TRUE, _T("txt"), _T("烧写数据"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),this);

		CString pathName = "";
		int retval = fileDlg.DoModal();
		if(retval==IDOK)
		{
			CString sFilename = fileDlg.GetPathName();
			m_FileAddress.SetWindowText(sFilename);

			bool bFlag = false;

			if (m_pSubStation->m_WriteUpdateMap.size() >0 )
			{
				std::map<int, WriteDataFromFile*>::iterator It;
				for (It = m_pSubStation->m_WriteUpdateMap.begin(); It !=m_pSubStation->m_WriteUpdateMap.end(); It ++)
				{
					if (It->second)
						delete It->second;
				}
				m_pSubStation->m_WriteUpdateMap.clear();
			}

			while (m_pSubStation->m_pWriteUpdateList.begin() != m_pSubStation->m_pWriteUpdateList.end())
			{
				m_pSubStation->m_pWriteUpdateList.erase(m_pSubStation->m_pWriteUpdateList.begin());
			}

			try
			{
 				CStdioFile file;

				if (!file.Open(sFilename,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText|CFile::shareDenyWrite))
				{
					AfxMessageBox("File not be Open!",MB_OK);
					file.Close();
					return;
				}

				file.SeekToBegin();
				m_pSubStation->ClearWriteUpdateList();

				CString strLine="";
				WriteDataFromFile *pdata = NULL;
				int nIdx = 0;
				int nRowIdx = 0;
				int nPackNo = 0;
				while (file.ReadString(strLine))
				{
					strLine.Trim();
					if (strLine.IsEmpty()) 
						continue;

					if (strLine.Left(1) == "@")
					{
						if (pdata == NULL)
						{
							bFlag = false;
							pdata = new WriteDataFromFile();
							pdata->m_strCodeAddr = strLine.Mid(1, strLine.GetLength()-1);
							nIdx = 0;
							continue;
						}
						else
						{
							bFlag = false;
							pdata->m_nBufLen = nIdx;
							pdata->m_nPackNo = nPackNo;
							

							m_pSubStation->m_pWriteUpdateList.push_back(pdata);
							m_pSubStation->m_WriteUpdateMap.insert(PAIR(nPackNo, pdata));
							nPackNo++;

							pdata = new WriteDataFromFile();
							nRowIdx = 0;
							pdata->m_strCodeAddr = strLine.Mid(1, strLine.GetLength()-1);
							nIdx = 0;
							continue;
							
						}
					}
					else if (strLine.Left(1) == "q")
					{
// 						if (bFlag)
// 						{
// 							int stCode = strtol(pdata->m_strCodeAddr, NULL, 16);
// 							stCode = stCode + 0xA0;
// 							CString temp = "";
// 							temp.Format("%04X", stCode);
// 							pdata->m_strCodeAddr = temp;
// 						}

						pdata->m_nBufLen = nIdx;
						pdata->m_nPackNo = nPackNo;			
						m_pSubStation->m_pWriteUpdateList.push_back(pdata);
						m_pSubStation->m_WriteUpdateMap.insert(PAIR(nPackNo, pdata));
						nPackNo++;
						break;
					}
					else
					{
						if (pdata != NULL)
						{
							if (nRowIdx < 10)
							{
								std::list<CString> list;
								CString resToken;
								int curPos = 0;

								resToken = strLine.Tokenize(_T(" "), curPos);
								while (resToken != _T(""))
								{
									list.push_back(resToken);
									resToken = strLine.Tokenize(_T(" "), curPos);
								}

								std::list<CString>::iterator it_list;

								CString text = "";
								for (it_list = list.begin(); it_list!=list.end(); it_list++)
								{
									text = (CString)(*it_list);
									pdata->m_CodeContext[nIdx] = strtol(text, NULL, 16);
									nIdx++;
								}

								nRowIdx++;
							}
							else
							{
								bFlag = true;
								pdata->m_nBufLen = nIdx;
								pdata->m_nPackNo = nPackNo;

								m_pSubStation->m_pWriteUpdateList.push_back(pdata);
								m_pSubStation->m_WriteUpdateMap.insert(PAIR(nPackNo, pdata));
								nPackNo++;

								int stCode = strtol(pdata->m_strCodeAddr, NULL, 16);
								stCode = stCode + 0xA0;
								CString temp = "";
								temp.Format("%04X", stCode);

								pdata = new WriteDataFromFile();
								pdata->m_strCodeAddr = temp;
								nIdx = 0;
								nRowIdx = 0;

								std::list<CString> list;
								CString resToken;
								int curPos = 0;

								resToken = strLine.Tokenize(_T(" "), curPos);
								while (resToken != _T(""))
								{
									list.push_back(resToken);
									resToken = strLine.Tokenize(_T(" "), curPos);
								}

								std::list<CString>::iterator it_list;

								CString text = "";
								for (it_list = list.begin(); it_list!=list.end(); it_list++)
								{
									text = (CString)(*it_list);
									pdata->m_CodeContext[nIdx] = strtol(text, NULL, 16);
									nIdx++;
								}

								nRowIdx++;

							}
							
						}
						else
						{

						}
					}
				}

				m_nPackCntSum = m_pSubStation->m_pWriteUpdateList.size();
				InitGridItems();
				m_pSubStation->m_CurrentWriteUpdateOp = WriteUpdate_Read;
				m_HasUpdateFile = true;

			}
			catch (CFileException* e)
			{
				e->ReportError(MB_OK);
				e->Delete();
				return;
			}
		}
	}
	else
	{
		m_HasUpdateFile = false;

    }
}

void CRtuAttrWriteUpdate::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSubStation)
	{
		WriteDataFromFile *pData = NULL;
		if (!m_pSubStation->bIteratorVaild)
		{
			return;
		}

		if (m_bCheckBoxChecked)
		{
			int n = m_pSubStation->m_WriteUpdateMap.size();
			if (0 == n)
			{
				return ;
			}

			m_pSubStation->m_it_map = m_pSubStation->m_WriteUpdateMap.begin();
			if (m_pSubStation->m_it_map == m_pSubStation->m_WriteUpdateMap.end())
			{
				return;
			}
		}

		pData = (WriteDataFromFile*)(m_pSubStation->m_it_map->second);
		if (pData != NULL)
		{
			m_bIsUpdating = true;
			m_pSubStation->m_CurrentWriteUpdateOp = WriteUpdate_Send;
			SetWndsEnable(TRUE, 1);
			m_blResult = FALSE;

			CWriteFrame writeFrame;
			m_pSubStation->BuildBasicPacket(writeFrame);  //生成初始的下发帧报文帧数据信息

			writeFrame.Sendbuf[0] = '$';
			writeFrame.Sendbuf[1] = '$';
			writeFrame.Sendbuf[2] = 27+pData->m_nBufLen;

			CString strStCode = writeFrame.m_nDevAdddr;
			char charStCode[11];
			memset(charStCode, 0, 10);
			memcpy(charStCode, strStCode.GetBuffer(10), 10);

			writeFrame.Sendbuf[3] = charStCode[0];
			writeFrame.Sendbuf[4] = charStCode[1];
			writeFrame.Sendbuf[5] = charStCode[2];
			writeFrame.Sendbuf[6] = charStCode[3];
			writeFrame.Sendbuf[7] = charStCode[4];
			writeFrame.Sendbuf[8] = charStCode[5];
			writeFrame.Sendbuf[9] = charStCode[6];
			writeFrame.Sendbuf[10] = charStCode[7];
			writeFrame.Sendbuf[11] = charStCode[8];
			writeFrame.Sendbuf[12] = charStCode[9];


			writeFrame.Sendbuf[13] = 0xA0;//命令码

			WORD PackCnt = pData->m_nPackNo;

			writeFrame.Sendbuf[14] = HIBYTE(PackCnt);
			writeFrame.Sendbuf[15] = LOBYTE(PackCnt);
			writeFrame.Sendbuf[16] = HIBYTE(m_nPackCntSum);
			writeFrame.Sendbuf[17] = LOBYTE(m_nPackCntSum);

			m_pSubStation->m_CurrUpdataPackCnt++;

			CString strCodeAddr = pData->m_strCodeAddr;
			int nCodeAddr = strtol(strCodeAddr, NULL, 16);

			char charCodeAddr[5];
			memset(charCodeAddr, 0, 4);
			memcpy(charCodeAddr, &nCodeAddr, 4);

			writeFrame.Sendbuf[18] = charCodeAddr[3];
			writeFrame.Sendbuf[19] = charCodeAddr[2];
			writeFrame.Sendbuf[20] = charCodeAddr[1];
			writeFrame.Sendbuf[21] = charCodeAddr[0];

			writeFrame.Sendbuf[22] = pData->m_nBufLen;
			memcpy(writeFrame.Sendbuf+23, pData->m_CodeContext, pData->m_nBufLen);
			WORD CRC = CUdfGlobalFUNC::CreateCRC_Write(writeFrame.Sendbuf, 23+pData->m_nBufLen);
			writeFrame.Sendbuf[23+pData->m_nBufLen] = HIBYTE(CRC);
			writeFrame.Sendbuf[24+pData->m_nBufLen] = LOBYTE(CRC);
			writeFrame.Sendbuf[25+pData->m_nBufLen] = '#';
			writeFrame.Sendbuf[26+pData->m_nBufLen] = '#';

			m_pSubStation->GetSysDataTrans_WriteUpdate()->SetRtuStation(m_pSubStation);
			m_pSubStation->GetSysDataTrans_WriteUpdate()->SendRtuLinkFrame(writeFrame.Sendbuf, 27+pData->m_nBufLen, writeFrame.m_CommInterface);
			//UINT nElapse = m_pSubStation->GetReSendInterval() * (RESEND_Count + 1)*1000;
			UINT nElapse = 15 * 1000;
			SetTimer(W_TIMER_WRITEUPDATE_SEND, nElapse, NULL);
		}
	}
}


void CRtuAttrWriteUpdate::OnBnClickedUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_UPDATE))
	{
		m_bCheckBoxChecked = true;
	}
	else
	{
		m_bCheckBoxChecked = false;
	}
}

void CRtuAttrWriteUpdate::OnBnClickedCancle()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(W_TIMER_WRITEUPDATE_SEND);
	KillTimer(W_TIMER_WRITEUPDATE_READ);
	KillTimer(W_TIMER_WRITEUPDATE_START);

	m_pSubStation->m_it_map = m_pSubStation->m_WriteUpdateMap.begin();
	if (m_pSubStation->m_it_map == m_pSubStation->m_WriteUpdateMap.end())
	{
		m_pSubStation->bIteratorVaild = false;
	}
	else
	{
		m_pSubStation->bIteratorVaild = true;
	}

	SetWndsEnable(FALSE, 1);

	return;
}

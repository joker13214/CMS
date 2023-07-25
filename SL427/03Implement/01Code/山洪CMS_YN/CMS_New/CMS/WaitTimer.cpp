#include "StdAfx.h"
#include "WaitTimer.h"

CWaitTimer::CWaitTimer()
{
	m_hTimer=NULL;
	InitWaitTimer();
}

CWaitTimer::~CWaitTimer()
{
	UninitWaitTimer();
}

void CWaitTimer::InitWaitTimer()
{
	if(m_hTimer==NULL)
		m_hTimer = CreateWaitableTimer(NULL,TRUE,NULL);
	SetWaitTimer(0);
}

void CWaitTimer::UninitWaitTimer()
{
	if(m_hTimer!=NULL)
	{
		CloseHandle(m_hTimer);
	}
}

void CWaitTimer::SetWaitTimer(__int64 millionSeconds)
{
	if(m_hTimer==NULL)
		InitWaitTimer();
	LARGE_INTEGER lDueTime;
	lDueTime.QuadPart=-10000*millionSeconds;
	SetWaitableTimer(m_hTimer,&lDueTime,0,0,0,0);
}

bool CWaitTimer::WaitTimerState(unsigned long millionSeconds)
{	
	if(m_hTimer==NULL)
		return false;
	if(WaitForSingleObject(m_hTimer,millionSeconds)==WAIT_OBJECT_0)
		return true;
	else
		return false;
}



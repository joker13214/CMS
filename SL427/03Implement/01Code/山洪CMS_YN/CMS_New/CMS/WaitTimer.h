#pragma once

//¶¨Ê±Æ÷Àà
class CWaitTimer
{
public:
	CWaitTimer();
	virtual ~CWaitTimer();
	void InitWaitTimer();

	void SetWaitTimer(__int64 millionSeconds);
	bool WaitTimerState(unsigned long millionSeconds=0);
private:
	HANDLE m_hTimer;
	void UninitWaitTimer();
};

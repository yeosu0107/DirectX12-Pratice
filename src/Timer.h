#pragma once

const ULONG MAX_SAMPLE_COUNT = 50; 
// 50ȸ�� ������ ó���ð��� �����Ͽ� ����Ѵ�

class CGameTimer
{
private:
	bool m_bHardwareHasPerformanceCounter;   
	float m_fTimeScale; 
	float m_fTimeElapsed;    
	__int64 m_nCurrentTime;    
	__int64 m_nLastTime; 
	__int64 m_nPerformanceFrequency;
	float m_fFrameTime[MAX_SAMPLE_COUNT];    
	ULONG m_nSampleCount;
	unsigned long m_nCurrentFrameRate; 
	unsigned long m_nFramesPerSecond; 
	float m_fFPSTimeElapsed;
public:
	CGameTimer();
	virtual ~CGameTimer();

	void Tick(float fLockFPS = 0.0f); //�ð�����
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nchar = 0);//�����ӷ���Ʈ ��ȯ
	float GetTimeElapsed() { return m_fTimeElapsed; } 
	//������ ��� ����ð� ��ȯ
	long getCurretFrame() {	return m_nCurrentFrameRate;	}
	
};

#pragma once

const ULONG MAX_SAMPLE_COUNT = 50; 
// 50회의 프레임 처리시간을 누적하여 평균한다

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

	void Tick(float fLockFPS = 0.0f); //시간갱신
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nchar = 0);//프레임레이트 반환
	float GetTimeElapsed() { return m_fTimeElapsed; } 
	//프레임 평균 경과시간 반환
	long getCurretFrame() {	return m_nCurrentFrameRate;	}
	
};

// Timer.h: the class for system timer

// Engine inlcude
#include <Windows.h>

class Timer 
{
public:
	// Default constructor
	Timer()
	{
		__int64 sysFreq;
		QueryPerformanceFrequency((LARGE_INTEGER*) &sysFreq);
		m_fSysFreq = (float) sysFreq;

		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)& currTime);
		m_llPrevTime = currTime;
		m_llCurrTime = currTime;
	}

	// run the timer
	void tick()
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*) &currTime);

		m_llPrevTime = m_llCurrTime;
		m_llCurrTime = currTime;
	}

	// get delta time in milliseconds
	const float getDeltaTime() const
	{
		return (m_llCurrTime - m_llPrevTime) * (1.0f / m_fSysFreq);
	}

	const float getCurrTime() const
	{
		return m_llCurrTime;
	}

private:
	long long					m_llCurrTime;
	long long					m_llPrevTime;
	float						m_fSysFreq;
};

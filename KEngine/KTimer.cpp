#include "KTimer.h"
float g_fSPF = 0.0f;

bool KTimer::Init()
{
	QueryPerformanceCounter(&m_liFrame);

	return true;
}
bool KTimer::Frame()
{
	QueryPerformanceCounter(&m_liCurrent);
	m_fSPF =
		static_cast<float>(m_liCurrent.QuadPart - m_liFrame.QuadPart)
		/ static_cast<float>(m_liFrequency.QuadPart);
	g_fSPF = m_fSPF;

	static	float fpsTime = 0.0f;
	m_iTmpCounter++;
	fpsTime += m_fSPF;

	if(fpsTime >= 1.0f)
	{
		m_iFPS = m_iTmpCounter;
		m_iTmpCounter = 0;
		fpsTime -= 1.0f;

		

	}

	m_fGameTimer += m_fSPF;
	m_liFrame = m_liCurrent;

	return true;
}
bool KTimer::Render()
{

	

	return true;
}
bool KTimer::Release()
{
	return true;
}
KTimer::KTimer()
{
	m_iTmpCounter = 0;
	m_iFPS = 0;
	m_fSPF = 0.0f;
	m_fGameTimer = 0.0f;
	QueryPerformanceFrequency(&m_liFrequency);

};
KTimer::~KTimer() {};
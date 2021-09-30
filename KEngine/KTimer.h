#pragma once
#include "KStd.h"
// 1,����ð� ����(�����÷��� �ð�)
// 2,1�������� ��� �ð�
// 3,1�ʿ� ����� ī����(FPS)
class KTimer
{
public:
	LARGE_INTEGER m_liFrequency;
	LARGE_INTEGER m_liCurrent;
	LARGE_INTEGER m_liFrame;
	int m_iTmpCounter;
	int m_iFPS;
	float m_fSPF;
	float m_fGameTimer;
	TCHAR m_szTimerString[MAX_PATH] = { 0, };
	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	KTimer();
	virtual ~KTimer();
};


#pragma once
#include <KCore.h>
#include <KObject.h>
class Sample : public KCore
{
public:
	KDebugCamera m_kCamera;
	float m_fYaw = 0.0f;
	float m_fPitch = 0.0f;

	KMap m_map;
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)override;
	
	
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	Sample();
};


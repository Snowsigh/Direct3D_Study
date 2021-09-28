#pragma once
#include"KDevice.h"

class KWindow : public KDevice
{
public:
	RECT m_rtWindow;
	RECT m_rtClient;

	HINSTANCE m_hInstance;
	HWND m_hWnd;
	bool m_bGameRun;
public:
	bool InitWindow(HINSTANCE hInstance, int nCmdShow, const WCHAR* strWindowTitle, int iWidth, int iHeight);
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	bool Run();
	virtual bool GameInit();
	virtual bool GameRun();
	virtual bool GameRelease();

public:
	KWindow(void);
	virtual ~KWindow(void);

};


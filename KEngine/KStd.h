#pragma once
#include<Windows.h>
#include <assert.h>
#include <tchar.h>
#pragma comment(lib,"KEngine.lib")
extern float g_fSPF;
extern HWND g_hWnd;
extern RECT g_rtClient;

template<class T>
class TSingleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};
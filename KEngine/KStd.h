#pragma once
#include<Windows.h>
#include <vector>
#include <assert.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <locale>
#include <math.h>
#include "KMartrix.h"


#pragma comment(lib,"KEngine.lib")

#define GAMEINIT int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int  nCmdShow)
#define GAMERUN(WindowName, Width, Height) {Sample g_Sample;g_Sample.InitWindow(hInstance,nCmdShow,	L#WindowName, Width, Height);g_Sample.Run();}
#define GAME(WindowName, Width, Height) GAMEINIT GAMERUN(WindowName, Width, Height)

extern float g_fGameTimer;
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
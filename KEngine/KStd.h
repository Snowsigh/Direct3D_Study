#pragma once
#include<Windows.h>
#include <vector>
#include <assert.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <locale>
#include <math.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <d3dcompiler.h>
#include <codecvt>
#include "KMartrix.h"
#include "SimpleMath.h"
#include "TMath.h"
#include "fbxsdk.h"

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"KEngine.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")
using namespace DirectX::SimpleMath;

extern float g_fGameTimer;
extern float g_fSPF;
extern HWND g_hWnd;
extern RECT g_rtClient;
extern ID3D11Device* g_pd3dDevice;

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
namespace KBASE
{
	//#include <codecvt>
	static std::wstring mtw(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(str);
	}
	static std::string wtm(std::wstring str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.to_bytes(str);
	}
	static TCHAR g_szDataPath[] = L"../../Data/";
	static TCHAR g_szPath[] = L"../../";

	static TMatrix DxConvertMatrix(TMatrix mat)
	{
		TMatrix tmp;
		tmp._11 = mat._11; tmp._12 = mat._13; tmp._13 = mat._12;
		tmp._21 = mat._31; tmp._22 = mat._33; tmp._23 = mat._32;
		tmp._31 = mat._21; tmp._32 = mat._23; tmp._33 = mat._22;
		tmp._41 = mat._41; tmp._42 = mat._43; tmp._43 = mat._42;
		tmp._14 = tmp._24 = tmp._34 = 0.0f;
		tmp._44 = 1.0f;
		return tmp;
	}

	static TMatrix ConvertMatrix(FbxMatrix& mat)
	{
		TMatrix tmp;

		float* pMatArray = reinterpret_cast<float*>(&tmp);
		double* pSrcArray = reinterpret_cast<double*>(&mat);
		for (int i = 0; i < 16; i++)
		{
			pMatArray[i] = (float)pSrcArray[i];
		}
		return tmp;
	}

	static TMatrix ConvertMatrixA(FbxAMatrix& mat)
	{
		TMatrix tmp;

		float* pMatArray = reinterpret_cast<float*>(&tmp);
		double* pSrcArray = reinterpret_cast<double*>(&mat);
		for (int i = 0; i < 16; i++)
		{
			pMatArray[i] = (float)pSrcArray[i];
		}
		return tmp;
	}

	static bool Compare(const std::pair<float, int>& a, const std::pair<float, int>& b)
	{
		if (a.first == b.first)
			return a.second > b.second;
		return a.first > b.first;
	}
	

}

#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

#define GAMEINIT int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int  nCmdShow)
#define GAMERUN(WindowName, Width, Height) {Sample g_Sample;g_Sample.InitWindow(hInstance,nCmdShow,	L#WindowName, Width, Height);g_Sample.Run();}
#define GAME(WindowName, Width, Height) GAMEINIT GAMERUN(WindowName, Width, Height)

#define HRFAILED if(FAILED(hr))return hr;
#define IFRELEASE(A) if(A)A->Release(); (A)=NULL;
#pragma once
#include "KStd.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )

struct TTextArray
{
	D2D1_MATRIX_3X2_F matWorld;
	D2D1_RECT_F rf;
	D3DCOLORVALUE color;
	TCHAR pText;
	//IDWriteTextFormat* pTextFormat;
	TTextArray()
	{
		matWorld = D2D1::IdentityMatrix();
		color = D2D1::ColorF(0, 0, 0, 1);
		rf.left = g_rtClient.left;
		rf.top = g_rtClient.top;
		rf.right = g_rtClient.right;
		rf.bottom = g_rtClient.bottom;
	}
};


class KWrite
{
public:
	std::vector<TTextArray>  m_TextList;
public:
	float					m_fDPIScaleX;
	float					m_fDPIScaleY;
	FLOAT					m_fdpiX;
	FLOAT					m_fdpiY;
	HWND					m_hWnd;
	ID2D1RenderTarget* m_pRT;
	ID2D1Factory* m_pD2DFactory;
	ID2D1SolidColorBrush* m_pBlackBrush;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	IDWriteTextLayout* m_pTextLayout;
public:
	DWRITE_FONT_WEIGHT		m_fontWeight;
	DWRITE_FONT_STYLE		m_fontStyle;
	BOOL					m_fontUnderline;
	std::wstring					m_wszFontFamily;
	float					m_fontSize;
	std::wstring					m_wszText;
	UINT32					m_cTextLength;
public:
	// �ʱ�ȭ
	bool			Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1* m_pSurface);
	bool			Init();
	bool			Render();
	// ������
	bool			Begin();
	HRESULT			DrawText(RECT rc, TCHAR* pText, D2D1::ColorF Color = D2D1::ColorF(1, 0, 0, 1));
	HRESULT			DrawText(D2D1_POINT_2F origin, D2D1::ColorF Color = D2D1::ColorF(1, 0, 0, 1));
	bool			End();
	// ��ü �Ҹ�
	bool			Release();
	// ��ü ���� �� �Ҹ�
	HRESULT			CreateDeviceIndependentResources();
	void			DiscardDeviceIndependentResources();
	HRESULT			CreateDeviceResources(IDXGISurface1* m_pSurface);
	void			DiscardDeviceResources();
public:
	HRESULT			SetText(D2D1_POINT_2F pos, wchar_t* text, D2D1::ColorF Color);
	HRESULT			SetFont(wchar_t* fontFamily);
	HRESULT			SetFontSize(float size);
	HRESULT			SetBold(bool bold);
	HRESULT			SetItalic(bool italic);
	HRESULT			SetUnderline(bool underline);

	// ȭ�� ������ ����
	void			OnResize(UINT width, UINT height, IDXGISurface1* pSurface);
public:
	KWrite();
	~KWrite();

};


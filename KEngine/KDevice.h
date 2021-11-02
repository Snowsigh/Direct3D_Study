#pragma once
#include "KInput.h"
#include "KViewRT.h"
class KDevice 
{
public:
	ID3D11Device* m_pd3dDevice;		// ����̽� ��ü
	IDXGISwapChain* m_pSwapChain;		// ����ü�� ��ü
	ID3D11RenderTargetView* m_pRenderTargetView;// ���� ����Ÿ�� ��
	D3D11_VIEWPORT			m_ViewPort;			// ����Ʈ 
	D3D_DRIVER_TYPE         m_driverType;		// ����̽� Ÿ��( ����Ʈ:�ϵ���� ���� )
	IDXGIFactory* m_pGIFactory;		// DXGI ��ü
	ID3D11DeviceContext* m_pImmediateContext;// ����̽� ���ؽ�Ʈ
	D3D_DRIVER_TYPE			m_DriverType;		// ����̽� Ÿ��
	D3D_FEATURE_LEVEL       m_FeatureLevel;		// DirectX�� ��ɼ���
	ID3D11DepthStencilView* m_pDepthStencilView; // depth
	ID3D11DepthStencilState* m_pDsvState; // depth
public:
	KViewDs m_DefaultDS;
	KViewRT m_DefaultRT;
public:
	void DX_CHECK(HRESULT hr, const TCHAR* funtion);
	HRESULT	CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight);
	HRESULT	SetRenderTargetView();
	HRESULT	SetViewPort();
public:
	bool CleanupDevice();
	bool SetDevice();
public:
	HRESULT SetDepthStencilView();
public:
	KDevice(void);
	virtual ~KDevice(void);

	

};


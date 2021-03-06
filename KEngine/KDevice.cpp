#include "KDevice.h"
ID3D11Device* g_pd3dDevice = nullptr;
HRESULT hr;
HRESULT KDevice::SetDepthStencilView()
{
	
	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	hr = m_DefaultDS.CreateDepthStencilView(Desc.BufferDesc.Width,
		Desc.BufferDesc.Height);
	return hr;
}
HRESULT KDevice::CreateDevice()
{
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		m_DriverType = driverTypes[0];
		hr = D3D11CreateDevice(
			NULL, m_DriverType, NULL, createDeviceFlags,
			featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &m_pd3dDevice, &m_FeatureLevel,
			&m_pImmediateContext);



	return hr;
}
HRESULT KDevice::CreateGIFactory()
{
	return CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_pGIFactory));
}
HRESULT KDevice::CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight)
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC sd; //Backbuffer
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = iWidth;
	sd.BufferDesc.Height = iHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain);
	HRFAILED
	return true;
}
HRESULT	KDevice::SetRenderTargetView()
{
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(hr = m_pSwapChain->GetBuffer(
		0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer), hr))
	{
		return hr;
	}
	m_DefaultRT.SetRenderTargetView(pBackBuffer);

	return hr;

}
HRESULT	KDevice::SetViewPort()
{

	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	// Setup the viewport    
	m_ViewPort.Width = (FLOAT)Desc.BufferDesc.Width;
	m_ViewPort.Height = (FLOAT)Desc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);

	return S_OK;
}
bool KDevice::CleanupDevice()
{
	m_DefaultDS.Release();
	m_DefaultRT.Release();
	
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	IFRELEASE(m_pRenderTargetView)
	IFRELEASE(m_pSwapChain)
	IFRELEASE(m_pImmediateContext)
	IFRELEASE(m_pd3dDevice)
	IFRELEASE(m_pGIFactory)
	IFRELEASE(m_pDepthStencilView);
	IFRELEASE(m_pDsvState);

	m_pd3dDevice = NULL;
	m_pSwapChain = NULL;
	m_pRenderTargetView = NULL;
	m_pImmediateContext = NULL;
	m_pGIFactory = NULL;
	return true;
}
void KDevice::DX_CHECK(HRESULT hr, const TCHAR* function)
{
	if (FAILED(hr))
	{
		LPWSTR output;
		WCHAR buffer[256] = { 0, };
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&output, 0, NULL);
		wsprintf(buffer, L"File=%ls\nLine=%d\nFuction=%ls", _T(__FILE__), __LINE__, function);
		MessageBox(NULL, buffer, output, MB_OK);
	}
}
bool KDevice::SetDevice()
{
	if (FAILED(CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(CreateDevice()))
	{
		return false;
	}
	g_pd3dDevice = m_pd3dDevice;
	if (FAILED(CreateSwapChain(g_hWnd,
		g_rtClient.right,
		g_rtClient.bottom)))
	{
		return false;
	}
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(SetDepthStencilView()))
	{
		return false;
	}
	if (FAILED(SetViewPort()))
	{
		return false;
	}
	return true;

}
KDevice::KDevice(void)
{


	m_pd3dDevice = NULL;		
	m_pSwapChain = NULL;		
	m_pRenderTargetView = NULL;		
	m_driverType = D3D_DRIVER_TYPE_NULL;			
	m_pImmediateContext = NULL;		
	m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;		
}
KDevice::~KDevice(void)
{

}
#include "KViewRT.h"

ID3D11Texture2D* KViewRT::CreateTexture(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// 1)�ؽ�ó ���� : ����,���ٽ� ���� �����ϴ� ���ۿ�
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC td;
	td.Width = Width;
	td.Height = Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	hr = g_pd3dDevice->CreateTexture2D(&td, NULL, &pTexture);
	if (FAILED(hr))
	{
		return nullptr;
	}
	return pTexture;
}

HRESULT KViewRT::SetRenderTargetView(ID3D11Texture2D* pTexture)
{
	HRESULT hr = S_OK;

	hr = g_pd3dDevice->CreateRenderTargetView(
		pTexture, NULL,
		&m_pRenderTargetView);
	if (FAILED(hr))
	{
		pTexture->Release();
		return hr;
	}

	pTexture->Release();
	return hr;
}

HRESULT KViewRT::CreateRenderTargetView(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// 1)�ؽ�ó ���� : ����,���ٽ� ���� �����ϴ� ���ۿ�
	m_pDSTexture = CreateTexture(Width, Height);
	if (m_pDSTexture == nullptr)
	{
		return E_FAIL;
	}
	hr = g_pd3dDevice->CreateShaderResourceView(m_pDSTexture, NULL, &m_pTextureSRV);
	if (FAILED(hr))
	{
		m_pDSTexture->Release();
		return hr;
	}
	hr = g_pd3dDevice->CreateRenderTargetView(
		m_pDSTexture, NULL,
		&m_pRenderTargetView);
	if (FAILED(hr))
	{
		m_pDSTexture->Release();
		return hr;
	}
	return hr;
}

bool KViewRT::Release()
{
	IFRELEASE(m_pDSTexture);
	IFRELEASE(m_pTextureSRV);
	IFRELEASE(m_pRenderTargetView);
	return true;
}
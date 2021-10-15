#include "KTexture.h"

bool KTexture::Init()
{
    return false;
}

bool KTexture::Frame()
{
    return false;
}

bool KTexture::Render()
{
    return false;
}

bool KTexture::Release()
{
    if (m_pTexture) m_pTexture->Release();
    if (m_pTextureSRV) m_pTextureSRV->Release();
    if (m_pSampler) m_pSampler->Release();
    m_pTexture = nullptr;
    m_pTextureSRV = nullptr;
    m_pSampler = nullptr;
    return true;
}

bool KTexture::LoadTexture(std::wstring texFileName)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(g_pd3dDevice, texFileName.c_str(), &m_pTexture, &m_pTextureSRV);
    if (FAILED(hr))
    {
        hr = DirectX::CreateDDSTextureFromFile(g_pd3dDevice, texFileName.c_str(), &m_pTexture, &m_pTextureSRV);
    }
    HRFAILED
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = g_pd3dDevice->CreateSamplerState(&sd, &m_pSampler);
    HRFAILED
    return true;
}
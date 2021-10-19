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
    IFRELEASE(m_pTexture)
    IFRELEASE(m_pTextureSRV)
        IFRELEASE(m_pSampler)
        m_pSampler = nullptr;
    m_pTexture = nullptr;
    m_pTextureSRV = nullptr;
    return true;
}

bool KTexture::LoadTexture(std::wstring texFileName)
{
    HRESULT hr = DirectX::CreateDDSTextureFromFile(g_pd3dDevice, texFileName.c_str(), &m_pTexture, &m_pTextureSRV);
    if (FAILED(hr))
    {
        hr = DirectX::CreateWICTextureFromFile(g_pd3dDevice, texFileName.c_str(), &m_pTexture, &m_pTextureSRV);
    }
    HRFAILED

    m_pSampler =  CreateSampler();
    return true;
}

ID3D11SamplerState* KTexture::CreateSampler()
{
    ID3D11SamplerState* pSampler;
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
   g_pd3dDevice->CreateSamplerState(&sd, &pSampler);
   return pSampler;

}

KTexture::KTexture()
{
    m_pTexture = nullptr;
    m_pTextureSRV = nullptr;
    m_pSampler = nullptr;
}

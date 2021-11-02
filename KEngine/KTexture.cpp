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
    return true;
}


KTexture::KTexture()
{
    m_pTexture = nullptr;
    m_pTextureSRV = nullptr;
}

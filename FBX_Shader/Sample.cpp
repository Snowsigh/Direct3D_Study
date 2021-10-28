#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    TPlane p;
    p.x = 0.0f;
    p.y = 1.0f;
    p.z = 0.0f;
    p.w = 0.0f;
    TVector4 v;
    v.x = 100.0f;
    v.y = 100.0f;
    v.z = 100.0f;
    v.w = 0.0f;
    D3DXMatrixShadow(&m_matShadow, &v, &p);

    ID3DBlob* PSBlob = nullptr;
    PSBlob = KModel::LoadShaderBlob(L"../../Data/Shader/CharacterShader.txt",
        "PSShadow", "ps_5_0");

    if (PSBlob != nullptr)
    {
        HRESULT hr = S_OK;
        hr = g_pd3dDevice->CreatePixelShader(
            PSBlob->GetBufferPointer(),
            PSBlob->GetBufferSize(),
            NULL, &m_pPSShadow);
        if (FAILED(hr)) return hr;
        PSBlob->Release();
    }

    m_KObj.LoadObject("../../Data/Object/man.FBX",L"../../Data/Shader/CharacterShader.txt",m_pImmediateContext);
    SetUpCamera(TVector3(0,0,-30),TVector3(0,0,0));
    

    return true;
}
bool	Sample::Frame()
{
    m_KObj.Frame();
    return true;
}
bool	Sample::Render()
{
    
    m_KObj.SetMatrixNoTranspose(&m_KObj.m_matWorld, &m_kCamera.m_matView, &m_kCamera.m_matProj);
    m_KObj.SetPixelShader(nullptr);
    m_KObj.Render();
    m_KObj.SetMatrixNoTranspose(&m_matShadow, &m_kCamera.m_matView, &m_kCamera.m_matProj);
    m_KObj.SetPixelShader(m_pPSShadow);
    m_KObj.Render();
    
   
    
    return true;
}
bool	Sample::Release()
{
    m_KObj.Release();
    IFRELEASE(m_pPSShadow);
    return true;
}



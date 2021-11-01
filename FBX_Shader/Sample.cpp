#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    m_kMini.Create(m_pImmediateContext, L"../../Data/Shader/PlaneVS.txt", L"../../Data/Shader/PlanePS.txt");
    m_kPlane.Create(m_pImmediateContext, L"../../Data/Shader/PlaneVS.txt", L"../../Data/Shader/PlanePS.txt", L"../../Data/air.bmp");
    TMatrix matWorld, matScale;
    D3DXMatrixRotationX(&matWorld, XM_PI / 2.0f);
    D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
    m_kPlane.m_matWorld = matScale * matWorld;
    m_kRt.Create(4096, 4096);

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
    if (g_Input.GetKey(VK_UP) >= KEY_PUSH)
    {
        m_vMoePos.z += g_fSPF * 100.0f;
    }
    if (g_Input.GetKey(VK_DOWN) >= KEY_PUSH)
    {
        m_vMoePos.z -= g_fSPF * 100.0f;
    }
    if (g_Input.GetKey(VK_LEFT) >= KEY_PUSH)
    {
        D3DXMatrixRotationY(&m_KObj.m_matWorld, g_fGameTimer);
        D3DXMatrixRotationY(&m_matShadow, g_fGameTimer);
    }
    m_KObj.m_matWorld._41 = m_vMoePos.x;
    m_KObj.m_matWorld._42 = m_vMoePos.y;
    m_KObj.m_matWorld._43 = m_vMoePos.z;



    

    return true;
}
bool	Sample::Render()
{
    m_pImmediateContext->RSSetState(m_pRsSolid);
    
    if (m_kRt.Begin(m_pImmediateContext))
    {
        m_KObj.SetMatrixNoTranspose(&m_KObj.m_matWorld, &m_kCamera.m_matView, &m_kCamera.m_matProj);
        m_KObj.SetPixelShader(m_pPSShadow);
        m_KObj.Render();
        m_kRt.End(m_pImmediateContext);
    }
    
    m_kPlane.SetMatrix(&m_kPlane.m_matWorld, &m_kCamera.m_matView, &m_kCamera.m_matProj);
    m_kPlane.Render();

    m_kMini.SetMatrix(nullptr, nullptr, nullptr);
    m_kMini.PreRender();
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_kRt.m_pTextureSRV);
    m_kMini.PostRender(m_kMini.m_iNumIndex);

    m_KObj.SetMatrixNoTranspose(&m_KObj.m_matWorld, &m_kCamera.m_matView, &m_kCamera.m_matProj);
    m_KObj.SetPixelShader(nullptr);
    m_KObj.Render();
    
   
    
    return true;
}
bool	Sample::Release()
{
    m_kMini.Release();
    m_kPlane.Release();
    m_kRt.Release();
    m_KObj.Release();
    IFRELEASE(m_pPSShadow);
    return true;
}



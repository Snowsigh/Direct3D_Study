#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    m_matTex._11 = 0.5f; m_matTex._22 = -0.5f; // x * 0.5, y * -0.5
    m_matTex._41 = 0.5f; m_matTex._42 = 0.5f; // x + 0.5, y + 0.5

    m_kLight1.Set(TVector3(100, 150, 100), TVector3(0, 0, 0));

    m_kMini.Create(m_pImmediateContext, L"../../Data/Shader/PlaneVS.txt", L"../../Data/Shader/PlanePS.txt");
    m_kPlane.Create(m_pImmediateContext, L"../../Data/Shader/ProjShader.txt", L"../../Data/Shader/ProjShader.txt", L"../../Data/air.bmp");
    TMatrix matWorld, matScale;
    D3DXMatrixRotationX(&matWorld, XM_PI / 2.0f);
    D3DXMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
    m_kPlane.m_matWorld = matScale * matWorld;
    m_kRt.Create(4096, 4096);


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
    SetUpCamera(TVector3(0,0,-30),TVector3(0,0,0), 300.0f);


    

    return true;
}
bool	Sample::Frame()
{
    
    m_KObj.Frame();
    m_kLight1.Frame();
    m_kbShadow.g_matShadow1 = m_kLight1.m_matView * m_kLight1.m_matProj * m_matTex;
    
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

    ApplyRS(m_pImmediateContext, KDxState::g_pRSSolid);
    
    if (m_kRt.Begin(m_pImmediateContext))
    {
        m_KObj.SetMatrixNoTranspose(&m_KObj.m_matWorld, &m_kLight1.m_matView, &m_kLight1.m_matProj);
        m_KObj.SetPixelShader(m_pPSShadow);
        m_KObj.Render();
        m_kRt.End(m_pImmediateContext);
    }
    ApplySS(m_pImmediateContext, KDxState::g_pClampSS,1);

    m_kPlane.m_kbData.matNormal = m_kbShadow.g_matShadow1;

    m_kPlane.SetMatrix(&m_kPlane.m_matWorld, &m_kCamera.m_matView, &m_kCamera.m_matProj, &m_kPlane.m_kbData.matNormal);
    m_pImmediateContext->PSSetShaderResources(1, 1, &m_kRt.m_pTextureSRV);
    m_kPlane.Render();

    m_kMini.SetMatrix(nullptr, nullptr, nullptr,nullptr);
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



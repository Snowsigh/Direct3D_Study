#include "Sample.h"

GAME(KGCA, 800, 600);
LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
bool	Sample::Init()
{
    KMapinfo info{
        64 + 1,
        64 + 1, 0, 0, 0,
        1.0f
    };
    if (m_kMap.Load(info))
    {
        m_kMap.Init(m_pImmediateContext);
    }
    m_kQuadtree.LoadObject(L"../../Data/Script/StaticLod.txt");
    m_kQuadtree.Build(&m_kMap);
    m_kQuadtree.GetCamera(&m_kCamera);

    m_kCamera.Init();
    m_kCamera.CreateViewMatrix(TVector3(16, 3, -31), TVector3(16, 3, 10000));
    m_kCamera.CreateProjMatrix(1.0f, 1000.0f, TBASIS_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_BACK;
    m_pd3dDevice->CreateRasterizerState(&rd, &m_pRsWire);


    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    m_pd3dDevice->CreateRasterizerState(&rd, &m_pRsSolid);

    return true;
}
bool	Sample::Frame()
{
    if (g_Input.GetKey('O') == KEY_UP)
    {

        m_bWiremod = !m_bWiremod;

    }
    if (m_bWiremod)
    {
        m_pImmediateContext->RSSetState(m_pRsWire);
    }
    else
    {
        m_pImmediateContext->RSSetState(m_pRsSolid);
    }


    m_kCamera.Frame();
    m_kMap.Frame();
    return true;
}
bool	Sample::Render()
{
    m_kMap.SetMatrix(
        nullptr,
        &m_kCamera.m_matView,
        &m_kCamera.m_matProj);
   // m_kMap.Render();
    m_kQuadtree.Render(m_pImmediateContext);
    return true;
}
bool	Sample::Release()
{
    //m_kMap.Release();
    m_kCamera.Release();

    m_pRsSolid->Release();
    m_pRsWire->Release();
    m_kQuadtree.Release();
    return true;
}



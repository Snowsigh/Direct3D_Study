#include "KCore.h"
LRESULT KCore::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
bool KCore::SetUpCamera(TVector3 vCameraPos, TVector3 vTargetPos)
{
    if (m_kCamera.Init(vCameraPos, vTargetPos)) return true;
    return false;
}
bool KCore::SetWireFrame()
{
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
bool	KCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	KCore::GameInit()
{
    KDevice::SetDevice();
    SetWireFrame();
    m_kTimer.Init();
    g_Input.Init();
    m_kWrite.Init();
   //m_kModel.Init(m_pImmediateContext);
    
    IDXGISurface1* m_pBackBuffer;
    m_pSwapChain->GetBuffer(0,
        __uuidof(IDXGISurface),
        (void**)&m_pBackBuffer);
    m_kWrite.CreateDeviceResources(m_pBackBuffer);
    if (m_pBackBuffer)m_pBackBuffer->Release();
    
    Init();
    return true;
}
void KCore::DebugFrame()
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

    if (g_Input.GetKey('1') == KEY_UP)
    {
        m_bDebugText = !m_bDebugText;
    }
    
}
bool	KCore::GameFrame()
{

    m_kTimer.Frame();
    g_Input.Frame();
    m_kWrite.Frame();
    DebugFrame();
    m_kCamera.Frame();
    //m_kModel.Frame();
        
    Frame();
    return true;
}
bool	KCore::GameRender()
{
    PreRender();
    m_kTimer.Render();
    m_kWrite.Render();
    g_Input.Render();
    
    //m_kModel.Render();
    
    Render();
    
    if (m_bDebugText)
    {
        RECT  rt = { 0, 0, 800, 600 };
        m_kWrite.DrawText(rt, m_kTimer.m_szTimerString,
            D2D1::ColorF(1, 1, 1, 1));
    }
    PostRender();

    return true;
}
bool	KCore::GameRelease()
{
    m_kTimer.Release();
    g_Input.Release();
    m_kWrite.Release();
    m_pRsSolid->Release();
    m_pRsWire->Release();
    m_kCamera.Release();
    //m_kModel.Release();
    CleanupDevice();
    Release();
    return true;
}

bool	KCore::Init()
{
    return true;
}
bool	KCore::Frame() {


    return true;
}
bool	KCore::PreRender(){

    float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 0.8f }; //red,green,blue,alpha
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    return true;
}
bool	KCore::Render() {
    
    return true;
}
bool	KCore::PostRender() {

    assert(m_pSwapChain);
    m_pSwapChain->Present(0, 0);
    return false;
}
bool	KCore::Release() {
    return true;
}
bool KCore::Run()
{
    GameInit();
    while (1)
    {
        if (!MsgRun())
        {
            break;
        }
        GameRun();
    }
    GameRelease();
    return true;
}
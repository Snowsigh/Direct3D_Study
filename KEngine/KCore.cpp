#include "KCore.h"

bool	KCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	KCore::GameInit()
{
    KDevice::SetDevice();
    
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
bool	KCore::GameFrame()
{

    m_kTimer.Frame();
    g_Input.Frame();
    m_kWrite.Frame();
    //m_kModel.Frame();

    if (g_Input.GetKey('1') == KEY_PUSH)
    {
        m_bDebugText = !m_bDebugText;
    }

    
    Frame();
    return true;
}
bool	KCore::GameRender()
{
    PreRender();
    m_kTimer.Render();
    g_Input.Render();
    m_kWrite.Render();
    //m_kModel.Render();
    if (m_bDebugText)
    {
        RECT  rt = { 0, 0, 800, 600 };
        m_kWrite.DrawText(rt, m_kTimer.m_szTimerString,
            D2D1::ColorF(1, 1, 1, 1));
    }
    Render();
    PostRender();

    return true;
}
bool	KCore::GameRelease()
{
    m_kTimer.Release();
    g_Input.Release();
    m_kWrite.Release();
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

    float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.8f }; //red,green,blue,alpha
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
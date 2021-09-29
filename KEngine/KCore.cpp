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
    Init();
    return true;
}
bool	KCore::GameFrame()
{
    m_kTimer.Frame();
    g_Input.Frame();
    Frame();
    return true;
}
bool	KCore::GameRender()
{
    PreRender();
    m_kTimer.Render();
    g_Input.Render();
    Render();
    PostRender();
    return true;
}
bool	KCore::GameRelease()
{
    m_kTimer.Release();
    g_Input.Release();
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

    float ClearColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f }; //red,green,blue,alpha
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
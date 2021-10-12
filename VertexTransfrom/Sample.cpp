#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    KMapinfo info{
        64 + 1,
        64 + 1, 0, 0, 0,
        1.0f
    };
    if (!m_kMap.Load(info, m_pImmediateContext, L"../../Data/Shader/VertexMap.txt", L"../../Data/Shader/PixelMap.txt"))
    {
        return false;
    }
    m_kQuadtree.LoadObject(L"../../Data/Script/StaticLod.txt");
    m_kQuadtree.Build(&m_kMap, &m_kCamera);
    SetUpCamera(TVector3(0,0,-30),TVector3(0,0,0));
    

    return true;
}
bool	Sample::Frame()
{
    m_kMap.Frame();
    return true;
}
bool	Sample::Render()
{
    m_kMap.SetMatrix(
        nullptr,
        &m_kCamera.m_matView,
        &m_kCamera.m_matProj);
   
    m_kQuadtree.Render(m_pImmediateContext);
    return true;
}
bool	Sample::Release()
{
    m_kQuadtree.Release();
    return true;
}



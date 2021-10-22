#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    m_obj.LoadObject("../../Data/Object/man.FBX", m_pImmediateContext);
    SetUpCamera(TVector3(0, 0, -30), TVector3(0, 0, 0));
    return true;
}
bool	Sample::Frame()
{
    m_obj.Frame();
    return true;
}
bool	Sample::Render()
{
    m_obj.SetMatrixNoTranspose(nullptr, &m_kCamera.m_matView, &m_kCamera.m_matProj);
    m_obj.Render();
    return true;
}
bool	Sample::Release()
{
    m_obj.Release();
    return true;
}



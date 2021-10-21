#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{

    m_kobj.LoadObject("../../Data/Object/multiCamera.FBX", m_pImmediateContext);
    SetUpCamera(TVector3(0,0,-30),TVector3(0,0,0));
    

    return true;
}
bool	Sample::Frame()
{

    return true;
}
bool	Sample::Render()
{
    m_kobj.SetMatrixNoTranspose(nullptr,
        &m_kCamera.m_matView,
        &m_kCamera.m_matProj);
    m_kobj.Render();
    return true;
}
bool	Sample::Release()
{
    m_kobj.Release();


    return true;
}


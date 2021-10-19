#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    //m_kTex.LoadTexture(L"../../Data/bitmap1.bmp");
    
    m_kFbxObj.LoadObject("../../Data/Object/multiCamera.FBX", m_pImmediateContext);
    
    SetUpCamera(TVector3(0,0,-50),TVector3(0,0,0));


    return true;
}
bool	Sample::Frame()
{

    return true;
}
bool	Sample::Render()
{
    
    m_kFbxObj.SetMatrixNoTranspose(nullptr,
        &m_kCamera.m_matView,
        &m_kCamera.m_matProj);
    m_kFbxObj.Render();


    return true;
}
bool	Sample::Release()
{
    m_kFbxObj.Release();

    return true;
}



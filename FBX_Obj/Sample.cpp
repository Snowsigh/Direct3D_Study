#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    //m_kTex.LoadTexture(L"../../Data/bitmap1.bmp");
    
    m_kFbxObj.LoadObject("../../Data/box.FBX", m_pImmediateContext);
    
    SetUpCamera(TVector3(0,0,-50),TVector3(0,0,0));
    
    m_pImmediateContext->PSSetSamplers(0, 1, &m_kFbxObj.m_kTexture.m_pSampler);
    m_pImmediateContext->PSSetShaderResources(1, 1, &m_kFbxObj.m_kTexture.m_pTextureSRV);

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
    m_kFbxObj.Render(m_pImmediateContext);


    return true;
}
bool	Sample::Release()
{
    m_kFbxObj.Release();

    return true;
}



#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    /*KMapinfo info{
        64 + 1,
        64 + 1, 0, 0, 0,
        1.0f
    };
    if (!m_kMap.Load(info, m_pImmediateContext, L"../../Data/Shader/VertexMap.txt", L"../../Data/Shader/PixelMap.txt"))
    {
        return false;
    }
    m_kQuadtree.LoadObject(L"../../Data/Script/StaticLod.txt");
    m_kQuadtree.Build(&m_kMap, &m_kCamera);*/
    m_kFbxObj.LoadObject("../../Data/box.FBX", m_pImmediateContext);
    m_kTex.LoadTexture(L"../../Data/bitmap1.bmp");
    SetUpCamera(TVector3(0,0,-50),TVector3(0,0,0));
    

    return true;
}
bool	Sample::Frame()
{
    m_kFbxObj.Frame();
   /* m_kMap.Frame();*/
    return true;
}
bool	Sample::Render()
{
    
    m_kFbxObj.SetMatrix(nullptr,
        &m_kCamera.m_matView,
        &m_kCamera.m_matProj);

    
  
    m_kFbxObj.PreRender();
    m_pImmediateContext->PSSetSamplers(0, 1, &m_kTex.m_pSampler);
    m_pImmediateContext->PSSetShaderResources(1, 1, &m_kTex.m_pTextureSRV);
   
    m_kFbxObj.PostRender(m_kFbxObj.m_VertexList.size());

    /*m_kMap.SetMatrix(
        nullptr,
        &m_kCamera.m_matView,
        &m_kCamera.m_matProj);
   
    m_kQuadtree.Render(m_pImmediateContext);*/
    return true;
}
bool	Sample::Release()
{
    m_kFbxObj.Release();
    m_kTex.Release();
    /*m_kQuadtree.Release();*/
    return true;
}



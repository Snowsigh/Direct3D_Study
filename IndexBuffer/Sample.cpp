#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    m_kCamera.Init();

    m_kCamera.CreateViewMatrix(KVector3(16, 3, -31),
        KVector3(16, 3, 100000));

    m_kCamera.CreateProjMatrix(1.0f,
        1000.0f, TBASIS_PI * 0.5f,
        (float)g_rtClient.right / (float)g_rtClient.bottom);



    KMapinfo info{
            128 + 1,
            128 + 1, 0,0, 0,
            1.0f
    };
    if (m_map.Load(info))
    {
        m_map.Init(m_pImmediateContext);
    }

 
   
    

    return true;
}
bool	Sample::Frame()
{
    m_kCamera.Frame();
    m_map.Frame();
    return true;
}
bool	Sample::Render()
{
    m_map.SetMatrix(nullptr, &m_kCamera.m_matView, &m_kCamera.m_matProj);
    m_kCamera.Render();
    m_map.Render();
    
    return true;
}
bool	Sample::Release()
{

    m_kCamera.Release();
    m_map.Release();
    return true;
}



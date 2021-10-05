#include "Sample.h"

GAME(KGCA, 800, 600);

LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
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
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;
    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    m_fYaw += g_fSPF * g_Input.m_pDrag.x * 5.0f;
    m_fPitch += g_fSPF * g_Input.m_pDrag.y * 5.0f;
    m_kCamera.Update(KVector4(m_fPitch, m_fYaw, 0.0f, 0.0f));
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
Sample::Sample()
{
   
}



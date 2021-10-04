#include "Sample.h"

GAME(KKOPROJECT, 800, 600);

bool	Sample::Init()
{
    KMapinfo info{
            64 + 1,
            64 + 1, 0,0, 0,
            1.0f
    };
    if (m_map.Load(info))
    {
        m_map.Init(m_pImmediateContext);
    }

    m_vCameraPos = { m_map.m_mapInfo.m_iNumCol / 2.0f, m_map.m_mapInfo.m_iNumCol / 2.0f, -(m_map.m_mapInfo.m_iNumRow / 2.0f) };
    m_vCameraTarget = { m_map.m_mapInfo.m_iNumCol / 2.0f, 0, -(m_map.m_mapInfo.m_iNumRow / 2.0f) + 1.0f };

    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_boxObj[iObj].LoadObject(L"ObjectData.txt");
        m_boxObj[iObj].Init(m_pImmediateContext);
    }
    return true;
}
Sample::Sample()
{
    m_fSpeed = 5.0f;
    m_vCameraPos = {8,5,-20.0f};
    m_vCameraTarget = {0,0,0.0f};
}
bool	Sample::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos.z += m_fSpeed * g_fSPF;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos.z -= m_fSpeed * g_fSPF;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_vCameraPos.x -= m_fSpeed * g_fSPF;
        m_vCameraTarget.x -= m_fSpeed * g_fSPF;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
        m_vCameraPos.x += m_fSpeed * g_fSPF;
        m_vCameraTarget.x += m_fSpeed * g_fSPF;
    }
    if (g_Input.GetKey('Q') >= KEY_PUSH)
    {
        m_vCameraTarget.x -= m_fSpeed * g_fSPF;
    }
    if (g_Input.GetKey('E') >= KEY_HOLD)
    {
        m_vCameraTarget.x += m_fSpeed * g_fSPF;
    }
    if (g_Input.GetKey('V') >= KEY_PUSH)
    {
        m_vCameraTarget.y += m_fSpeed * g_fSPF;
    }
    if (g_Input.GetKey('C') >= KEY_HOLD)
    {
        m_vCameraTarget.y -= m_fSpeed * g_fSPF;
    }


  //  m_kbData.matWorld = KMatrix::RotationZ(g_fGameTimer);

    KVector3 vUp = { 0,1,0.0f };
    m_kbData.matView = KMatrix::ViewLookAt(
        m_vCameraPos, m_vCameraTarget, vUp);
    m_kbData.matProj = KMatrix::PerspectiveFovLH(1.0f,
        1000.0f, TBASIS_PI * 0.5f,
        (float)g_rtClient.right / (float)g_rtClient.bottom);


    m_map.Frame();
    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_boxObj[iObj].Frame();
    }
    return true;
}
bool	Sample::Render()
{
    m_map.SetMatrix(
        &m_kbData.matWorld,
        &m_kbData.matView,
        &m_kbData.matProj);
    m_map.Render();

    //  m_boxObj[0].m_kbData.matWorld._31 = -3.0f;
   // m_boxObj[1].m_kbData.matWorld._31 = 3.0f;

    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_boxObj[iObj].SetMatrix(
            &m_boxObj[iObj].m_kbData.matWorld,
            &m_kbData.matView,
            &m_kbData.matProj);
        m_boxObj[iObj].Render();
    }
    return true;
}
bool	Sample::Release()
{
    m_map.Release();
    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_boxObj[iObj].Release();
    }
    return true;
}



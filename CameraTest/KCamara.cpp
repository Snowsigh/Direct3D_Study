#include "KCamara.h"

TMatrix KCamera::CreateViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
    m_vCameraPos = vPos;
    m_vCameraTarget = vTarget;
    D3DXMatrixLookAtLH(&m_matView,&m_vCameraPos,&m_vCameraTarget,&vUp);
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;
    return m_matView;
}

TMatrix KCamera::CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect)
{
    D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);
    return m_matProj;
}

bool KCamera::Init()
{
    return false;
}

bool KCamera::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
       /* m_vCameraPos.z += m_pSpeed * g_fSPF;*/
        m_vCameraPos = m_vCameraPos + m_vLook * m_pSpeed * g_fSPF;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        /*m_vCameraPos.z -= m_pSpeed * g_fSPF;*/
        m_vCameraPos = m_vCameraPos + m_vLook * -m_pSpeed * g_fSPF;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_vCameraPos.x -= m_pSpeed * g_fSPF;
        m_vCameraTarget.x -= m_pSpeed * g_fSPF;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
        m_vCameraPos.x += m_pSpeed * g_fSPF;
        m_vCameraTarget.x += m_pSpeed * g_fSPF;
    }
    if (g_Input.GetKey('Q') >= KEY_PUSH)
    {
        m_vCameraPos.y -= m_pSpeed * g_fSPF;
    }
    if (g_Input.GetKey('R') >= KEY_HOLD)
    {
        m_vCameraPos.y += m_pSpeed * g_fSPF;
    }

    m_matView = CreateViewMatrix(m_vCameraPos, m_vCameraTarget);

    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;
    return true;
}

bool KCamera::Render()
{
    return false;
}

bool KCamera::Release()
{
    return false;
}

KCamera::KCamera()
{
    m_pSpeed = 30.0f;
    m_vCameraPos = { 0, 20, -20.0f };
    m_vCameraTarget = { 0, 0, 1.0f };
}
TMatrix KDebugCamera::Update(KVector4 vValue)
{
   
    TQuaternion q;
    D3DXQuaternionRotationYawPitchRoll(&q, vValue.y, vValue.x, vValue.z);
    TMatrix matRotation;
    D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_vCameraPos);
    D3DXMatrixInverse(&m_matView, NULL, &matRotation);
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;

   
    return matRotation;
}

bool KDebugCamera::Frame()
{
     if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * m_pSpeed * g_fSPF;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * -m_pSpeed * g_fSPF;
    }
    return true;

}

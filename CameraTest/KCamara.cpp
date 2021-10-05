#include "KCamara.h"

KMatrix KCamera::CreateViewMatrix(KVector3 vPos, KVector3 vTarget, KVector3 vUp)
{
    m_vCameraPos = vPos;
    m_vCameraTarget = vTarget;
    m_matView = KMatrix::ViewLookAt(
        m_vCameraPos, m_vCameraTarget, vUp);
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

KMatrix KCamera::CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect)
{
    m_matProj = KMatrix::PerspectiveFovLH(fNear, fFar, fFov, fAspect);
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
        m_vCameraPos.z += m_pSpeed * g_fSPF;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos.z -= m_pSpeed * g_fSPF;
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
KMatrix KDebugCamera::Update(KVector4 vValue)
{
    TVector3 tmpPos = m_vCameraPos.ChanigeTK(m_vCameraPos);
    TMatrix tmpMat = m_matView.ChanigeTK(m_matView);
    TQuaternion q;
    D3DXQuaternionRotationYawPitchRoll(&q, vValue.y, vValue.x, vValue.z);
    TMatrix matRotation;
    D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &tmpPos);
    D3DXMatrixInverse(&tmpMat, NULL, &matRotation);
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;

    KMatrix tmpRotation = tmpRotation.ChanigeKT(matRotation);
    return tmpRotation;
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

    Vector3 vLook;
    vLook.x = m_vLook.x;
    vLook.y = m_vLook.y;
    vLook.z = m_vLook.z;
    KVector3 vTarget;
    vTarget.x = m_vCameraPos.x;
    vTarget.y = m_vCameraPos.y;
    vTarget.z = m_vCameraPos.z;

    Matrix matRotation, matY, matX;
    if (g_Input.GetKey(VK_RIGHT) >= KEY_PUSH)
    {
        matRotation = Matrix::CreateRotationY(g_fSPF);
    }
    if (g_Input.GetKey(VK_LEFT) >= KEY_PUSH)
    {
        matRotation = Matrix::CreateRotationY(-g_fSPF);
    }
    vLook = Vector3::Transform(vLook, matRotation);

    vTarget.x = vTarget.x + vLook.x * 100.0f;
    vTarget.y = vTarget.y + vLook.y * 100.0f;
    vTarget.z = vTarget.z + vLook.z * 100.0f;

    m_vCameraTarget.x = vTarget.x;
    m_vCameraTarget.y = vTarget.y;
    m_vCameraTarget.z = vTarget.z;



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

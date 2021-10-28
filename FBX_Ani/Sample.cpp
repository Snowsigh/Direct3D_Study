#include "Sample.h"

GAME(KGCA, 800, 600);

bool	Sample::Init()
{
    m_obj.LoadObject("../../Data/Object/multiCamera.FBX",L"../../Data/Shader/CharacterShader.txt", m_pImmediateContext);
    SetUpCamera(TVector3(0, 0, -30), TVector3(0, 0, 0));

    m_pObjectList.push_back(&m_obj);
    return true;
}
bool	Sample::Frame()
{
    for (int i = 0; i < m_pObjectList.size(); i++)
    {
        m_pObjectList[i]->Frame();
    }
    
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



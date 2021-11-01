#include "KShape.h"

bool KPlane::CreateVertexData()
{
    m_VertexList.resize(4);

    m_VertexList[0].pos = TVector3(-1.0f, 1.0f, 0.0f);
    m_VertexList[0].normal = TVector3(0.0f, 0.0f, -1.0f);
    m_VertexList[0].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_VertexList[0].tex = TVector2(0.0f, 0.0f);

    m_VertexList[1].pos = TVector3(1.0f, 1.0f, 0.0f);
    m_VertexList[1].normal = TVector3(0.0f, 0.0f, -1.0f);
    m_VertexList[1].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_VertexList[1].tex = TVector2(1.0f, 0.0f);


    m_VertexList[2].pos = TVector3(-1.0f, -1.0f, 0.0f);
    m_VertexList[2].normal = TVector3(0.0f, 0.0f, -1.0f);
    m_VertexList[2].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_VertexList[2].tex = TVector2(0.0f, 1.0f);


    m_VertexList[3].pos = TVector3(1.0f, -1.0f, 0.0f);
    m_VertexList[3].normal = TVector3(0.0f, 0.0f, -1.0f);
    m_VertexList[3].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_VertexList[3].tex = TVector2(1.0f, 1.0f);
    return true;
}

bool KPlane::CreateIndexData()
{
    m_IndexList.resize(6);
    m_IndexList[0] = 0;
    m_IndexList[1] = 1;
    m_IndexList[2] = 2;
    m_IndexList[3] = 2;
    m_IndexList[4] = 1;
    m_IndexList[5] = 3;

    return true;
}

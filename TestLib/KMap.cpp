#include "KMap.h"

bool KMap::Load(KMapinfo& pinfo)
{
    m_mapInfo = pinfo;
    m_mapInfo.m_iNumColCell = m_mapInfo.m_iNumCol - 1;
    m_mapInfo.m_iNumRowCell = m_mapInfo.m_iNumRow - 1;
    m_mapInfo.m_iNumVertex = m_mapInfo.m_iNumColCell * m_mapInfo.m_iNumRowCell * 6;
    return true;
}

bool KMap::CreateVertexData()
{
    //m_VertexList
    // 0  1  2
    // 3  4  5
    // 6  7  8

    m_VertexList.resize(m_mapInfo.m_iNumVertex);

    int iIndex = 0;
    for (int iRow = 0; iRow < m_mapInfo.m_iNumRowCell; iRow++)
    {
        for (int iCol = 0; iCol < m_mapInfo.m_iNumColCell; iCol++)
        {
            m_VertexList[iIndex + 0].pos.x = m_mapInfo.m_fCellDistance * iCol;
            m_VertexList[iIndex + 0].pos.y = 0.0f;
            m_VertexList[iIndex + 0].pos.z = -m_mapInfo.m_fCellDistance * iRow;

            m_VertexList[iIndex + 1].pos.x =
                m_VertexList[iIndex + 0].pos.x + m_mapInfo.m_fCellDistance;
            m_VertexList[iIndex + 1].pos.y = 0.0f;
            m_VertexList[iIndex + 1].pos.z =
                m_VertexList[iIndex + 0].pos.z;

            m_VertexList[iIndex + 2].pos.x =
                m_VertexList[iIndex + 0].pos.x;
            m_VertexList[iIndex + 2].pos.y = 0.0f;
            m_VertexList[iIndex + 2].pos.z =
                m_VertexList[iIndex + 0].pos.z - m_mapInfo.m_fCellDistance;

            m_VertexList[iIndex + 3].pos.x =
                m_VertexList[iIndex + 0].pos.x;
            m_VertexList[iIndex + 3].pos.y = 0.0f;
            m_VertexList[iIndex + 3].pos.z =
                m_VertexList[iIndex + 0].pos.z - m_mapInfo.m_fCellDistance;
            m_VertexList[iIndex + 4].pos.x =
                m_VertexList[iIndex + 0].pos.x + m_mapInfo.m_fCellDistance;
            m_VertexList[iIndex + 4].pos.y = 0.0f;
            m_VertexList[iIndex + 4].pos.z =
                m_VertexList[iIndex + 0].pos.z;
            m_VertexList[iIndex + 5].pos.x =
                m_VertexList[iIndex + 0].pos.x + m_mapInfo.m_fCellDistance;
            m_VertexList[iIndex + 5].pos.y = 0.0f;
            m_VertexList[iIndex + 5].pos.z =
                m_VertexList[iIndex + 0].pos.z - m_mapInfo.m_fCellDistance;

            m_VertexList[iIndex + 5].color =
                KVector4(
                    randstep(0.0f, 1.0f),
                    randstep(0.0f, 1.0f),
                    randstep(0.0f, 1.0f), 1.0f);
            iIndex += 6;
        }
    }
    if (m_VertexList.size() > 0) return true;
    return false;
}


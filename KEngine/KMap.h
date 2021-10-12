#pragma once
#include "KModel.h"
struct KMapinfo
{
    int m_iNumRow; 
    int m_iNumCol;
    int m_iNumRowCell;
    int m_iNumColCell;
    int m_iNumVertex;
    float m_fCellDistance;
};
class KMap : public KModel
{
public:
    KMapinfo m_mapInfo;
public:
    bool Load(KMapinfo& pinfo, ID3D11DeviceContext* pContext, LPCWSTR vsFile, LPCWSTR psFile);
    bool CreateVertexData() override;
    bool CreateIndexData() override;
    

};


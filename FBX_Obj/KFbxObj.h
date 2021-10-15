#pragma once
#include <KModel.h>
#include <KTexture.h>
#include <KStd.h>

class KFbxObj :
    public KModel
{
    FbxManager* m_pFbxManager;
    FbxImporter* m_pFbxImporter;
    FbxScene* m_pFbxScene;

    KTexture m_kTexture;
public:
    bool LoadObject(std::string filename, ID3D11DeviceContext* pContext);
    bool Create(ID3D11DeviceContext* pContext,LPCWSTR vsFile, LPCWSTR psFile) override;
    bool PostRender(UINT iNumIndex) override;
public:
    void	PreProcess(FbxNode* pNode);
    void	ParseNode(FbxNode* pNode);
    FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);

};


#pragma once
#include <KModel.h>
#include <KTexture.h>
#include <KStd.h>
#include "KMesh.h"

struct KMtrl
{
    FbxNode* m_pFbxNode;
    FbxSurfaceMaterial* m_pFbxMtrl;
    KTexture m_Texture;
    std::vector<KMtrl*> m_pSubMtrl;
    void Release()
    {
        m_Texture.Release();
        for (auto& data : m_pSubMtrl)
        {
            data->Release();
        }
    }
};

class KFbxObj :
    public KModel
{
    
    FbxManager* m_pFbxManager;
    FbxImporter* m_pFbxImporter;
    FbxScene* m_pFbxScene;
   
public:
    KTexture m_kTexture;

public:
    std::vector<FbxNode*> m_pFbxNodeList;
    std::vector<KMtrl*> m_pMtrlList;
    std::vector<KMesh*> m_pMeshList;
    KB_Data m_kbData;
public:
    bool LoadObject(std::string filename, ID3D11DeviceContext* pContext);
    bool Release();
public:
    void	SetMatrixNoTranspose(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
    bool    Render(ID3D11DeviceContext* pContext);
    int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
    void	LoadMaterial(KMtrl* pMtrl);
   
public:
    void	PreProcess(FbxNode* pNode);
    void	ParseNode(FbxNode* pNode, KMesh* pMesh);
    FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);

};


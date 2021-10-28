#pragma once
#include "KTexture.h"
#include "KFbxAni.h"
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
            
            delete data;
        }
        
    }
    KMtrl() {};
    KMtrl(FbxNode* pNode, FbxSurfaceMaterial* pFbxMtrl)
    {
        m_pFbxNode = pNode;
        m_pFbxMtrl = pFbxMtrl;
    }
};
class KFbxObj :
    public KModel
{   
    FbxManager* m_pFbxManager;
    FbxImporter* m_pFbxImporter;
    FbxScene* m_pFbxScene;

public: //�ؽ�ó �� �ִϸ��̼� Ŭ���� �ν��Ͻ�
    KTexture m_kTexture;
    KFbxAni m_kAni;
    TMatrix m_matWorld;
public: // ���׸���� �ڷ� ����Ʈ
    std::vector<FbxNode*> m_pFbxNodeList;
    std::vector<KMtrl*> m_pMtrlList;
    std::vector<KMesh*> m_pMeshList;
    KB_Data m_kbData;

public:
    bool LoadObject(std::string filename, LPCWSTR shadername, ID3D11DeviceContext* pContext);
    bool Release();
    TMatrix ParseTransform(FbxNode* pNode, TMatrix& matParent);
public:
    void	SetMatrixNoTranspose(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj);
    bool    Render();
    int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
    void	LoadMaterial(KMtrl* pMtrl);
public:
    void    SetPixelShader(ID3D11PixelShader* ps);
public:
    void	PreProcess(FbxNode* pNode);
    void	ParseNode(FbxNode* pNode, KMesh* pMesh);
    void	ParseMesh(FbxNode* pNode, KMesh* pMesh);
public:
    bool    Frame()override;
};


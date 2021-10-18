#pragma once
#include <KModel.h>

struct KLayer
{
	FbxLayerElementUV* pUV;
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementMaterial* pMaterial;
};
class KMesh : public KModel
{
	int m_iMtrlRef;
public:
	int					m_iNumLayer;
	std::vector<KLayer> m_LayerList;
	TMatrix				m_matWorld;
	std::vector<KMesh*> m_pSubMesh;

public:
	FbxColor ReadColor(const FbxMesh* mesh,
		DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh,
		DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
		int controlPointIndex, int iVertexIndex);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount,
		FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
public:
	INT GetRef()
	{
		return m_iMtrlRef;
	}
	bool SetRef(INT vValue)
	{
		m_iMtrlRef = vValue;
		return true;
	}
	bool Release() override
	{
		KModel::Release();
		for (auto data : m_pSubMesh)
		{
			data->Release();
			delete data;
		}
		return true;
	}


	
};


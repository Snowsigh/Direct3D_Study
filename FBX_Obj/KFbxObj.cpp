#include "KFbxObj.h"

bool KFbxObj::LoadObject(std::string filename, ID3D11DeviceContext* pContext)
{
    m_pFbxManager = FbxManager::Create();
    FbxIOSettings* ios = FbxIOSettings::Create(m_pFbxManager, IOSROOT);
    m_pFbxManager->SetIOSettings(ios);

	m_kTexture.m_pSampler = m_kTexture.CreateSampler();

    m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
    m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
    INT iFileFormat = -1;
    bool bRet = m_pFbxImporter->Initialize(filename.c_str(), iFileFormat, m_pFbxManager->GetIOSettings());
	bRet = m_pFbxImporter->Import(m_pFbxScene);
    FbxNode* m_pRootNode = m_pFbxScene->GetRootNode();
    PreProcess(m_pRootNode);

	for (int iMtrl = 0; iMtrl < m_pMtrlList.size(); iMtrl++)
	{
		KMtrl* pMtrl = m_pMtrlList[iMtrl];
		LoadMaterial(pMtrl);
	}

	for (int iObj = 0; iObj < m_pFbxNodeList.size(); iObj++)
	{
		FbxNode* pNode = m_pFbxNodeList[iObj];
		KMesh* pMesh = new KMesh;
		m_pMeshList.push_back(pMesh);
		ParseNode(pNode, pMesh);
		pMesh->Create(pContext,L"../../Data/Shader/FbxShader.txt", L"../../Data/Shader/FbxShader.txt");
	}
	return bRet;
}

void KFbxObj::SetMatrixNoTranspose(TMatrix* pMatWorld, TMatrix* pMatView, TMatrix* pMatProj)
{
	if (pMatWorld != nullptr)
	{
		m_kbData.matWorld = *pMatWorld;
	}
	if (pMatView != nullptr)
	{
		m_kbData.matView = *pMatView;
	}
	if (pMatProj != nullptr)
	{
		m_kbData.matProj = *pMatProj;
	}
}

bool KFbxObj::Render(ID3D11DeviceContext* pContext)
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		KMesh* pMesh = m_pMeshList[iObj];
		KMtrl* pKtrl = nullptr;
		if (pMesh->GetRef() >= 0)
		{
			pKtrl = m_pMtrlList[pMesh->GetRef()];
		}
		pMesh->SetMatrix(nullptr, &m_kbData.matView, &m_kbData.matProj);
		if (pKtrl != nullptr)
		{
			pContext->PSSetShaderResources(1, 1, &pKtrl->m_Texture.m_pTextureSRV);
		}
		pMesh->Render();
	}
	return true;
}

int KFbxObj::GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial)
{
	for (int iMtrl = 0; iMtrl < m_pMtrlList.size(); iMtrl++)
	{
		if (m_pMtrlList[iMtrl]->m_pFbxMtrl == pFbxMaterial)
		{
			return iMtrl;
		}
	}
	return -1;
	return 0;
}

void KFbxObj::LoadMaterial(KMtrl* pMtrl)
{
	FbxSurfaceMaterial* pFbxMaterial = pMtrl->m_pFbxNode->GetMaterial(0);
	FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (prop.IsValid())
	{
		int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
		for (int iTex = 0; iTex < iTexCount; iTex++)
		{
			FbxTexture* pTex = prop.GetSrcObject<FbxTexture>(iTex);
			if (pTex == nullptr) continue;
			FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTex);

			std::string szFileName;
			char Drive[MAX_PATH] = { 0, };
			char Dir[MAX_PATH] = { 0, };
			char FName[MAX_PATH] = { 0, };
			char Ext[MAX_PATH] = { 0, };
			if (fileTexture->GetFileName())
			{
				_splitpath_s(fileTexture->GetFileName(), Drive, Dir, FName, Ext);
				Ext[4] = 0;
				szFileName = FName;
				szFileName += Ext;
			}
			pMtrl->m_Texture.m_szFileName = KBASE::g_szDataPath;
			pMtrl->m_Texture.m_szFileName += KBASE::mtw(szFileName);
			pMtrl->m_Texture.LoadTexture(pMtrl->m_Texture.m_szFileName);
		}
	}

}

void KFbxObj::PreProcess(FbxNode* pNode)
{
    // pNode 정보 얻기
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	if (pFbxMaterial != nullptr)
	{
		KMtrl* pMtrl = new KMtrl;
		pMtrl->m_pFbxMtrl = pFbxMaterial;
		pMtrl->m_pFbxNode = pNode;
		m_pMtrlList.push_back(pMtrl);
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		FbxNodeAttribute::EType type =
			pChildNode->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			m_pFbxNodeList.push_back(pChildNode);
		}
		PreProcess(pChildNode);
	}
}

void KFbxObj::ParseNode(FbxNode* pNode, KMesh* pMesh)
{
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	pMesh->SetRef(GetRootMtrl(pFbxMaterial));
	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	std::vector< std::string> fbxFileTexList;
	if (pFbxMesh != nullptr)
	{
		int iNumLayer = pFbxMesh->GetLayerCount();
		std::vector< FbxLayerElementUV*> VertexUVList;
		// 정점성분 레이어 리스트
		for (int iLayer = 0; iLayer < iNumLayer; iLayer++)
		{
			FbxLayer* pLayer = pFbxMesh->GetLayer(0);
			if (pLayer->GetVertexColors() != nullptr) {}
			if (pLayer->GetNormals() != nullptr) {}
			if (pLayer->GetTangents() != nullptr) {}
			if (pLayer->GetUVs() != nullptr)
			{
				VertexUVList.push_back(pLayer->GetUVs());
			}
			if (pLayer->GetMaterials() != nullptr)
			{
				FbxLayerElementMaterial* fbxMaterial = pLayer->GetMaterials();
			}
		}

		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// 정점리스트 주소
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			// 삼각형, 사각형
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			int m_iNumTriangle = iPolySize - 2;
			int iCornerIndex[3];
			for (int iTriangle = 0;
				iTriangle < m_iNumTriangle;
				iTriangle++)
			{
				// 위치 인덱스
				iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
				iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
				iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);
				// UV 인덱스
				int u[3];
				u[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
				u[1] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
				u[2] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

				for (int iIndex = 0;
					iIndex < 3;
					iIndex++)
				{
					PNCT_VERTEX vertex;
					FbxVector4 pos = pVertexPositions[iCornerIndex[iIndex]];
					vertex.pos.x = (float)pos.mData[0];
					vertex.pos.y = (float)pos.mData[2];
					vertex.pos.z = (float)pos.mData[1];

					if (VertexUVList.size())
					{
						// todo : uvlist
						FbxLayerElementUV* pUVElement =
							VertexUVList[0];
						FbxVector2 uv = ReadTextureCoord(
							pFbxMesh, 1, pUVElement,
							iCornerIndex[iIndex], u[iIndex]);
						vertex.tex.x = (float)uv.mData[0];
						vertex.tex.y = (float)(1.0f - uv.mData[1]);
					}
					pMesh->m_VertexList.push_back(vertex);
				}
			}
		}
	}
}

FbxVector2 KFbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
{
	FbxVector2 uv(0, 0);
	if (dwVertexTextureCount < 1 || pUVSet == nullptr)
	{
		return uv;
	}
	int iVertexTextureCountLayer = pFbxMesh->GetElementUVCount();
	FbxLayerElementUV* pFbxLayerElementUV = pFbxMesh->GetElementUV(0);

	// 제어점은 평면의 4개 정점, 폴리곤 정점은 6개 정점을 위미한다.
	// 그래서 텍스처 좌표와 같은 레이어 들은 제어점 또는 정점으로 구분된다.
	switch (pUVSet->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint: // 제어점 당 1개의 텍스처 좌표가 있다.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: // 배열에서 직접 얻는다.
		{
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect: // 배열에 해당하는 인덱스를 통하여 얻는다.
		{
			int id = pUVSet->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	case FbxLayerElementUV::eByPolygonVertex: // 정점 당 1개의 매핑 좌표가 있다.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
	return uv;
}


bool KFbxObj::Release()
{
	
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		m_pMeshList[iObj]->Release();
	}
	for (int iObj = 0; iObj < m_pMtrlList.size(); iObj++)
	{
		m_pMtrlList[iObj]->Release();
		delete m_pMtrlList[iObj];
	}
	m_kTexture.Release();
	m_kTexture.SamplerRelease();
	return true;
}

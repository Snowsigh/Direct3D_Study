#include "KFbxObj.h"

bool KFbxObj::LoadObject(std::string filename, ID3D11DeviceContext* pContext)
{
	//IOSSETUP DEFAULT
    m_pFbxManager = FbxManager::Create();
	//m_kTexture.m_pSampler = m_kTexture.CreateSampler();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
    m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
    bool bRet = m_pFbxImporter->Initialize(filename.c_str());
	bRet = m_pFbxImporter->Import(m_pFbxScene);

    FbxNode* m_pRootNode = m_pFbxScene->GetRootNode();
    PreProcess(m_pRootNode);

	for (int iMtrl = 0; iMtrl < m_pMtrlList.size(); iMtrl++) // 머테리얼
	{
		KMtrl* pMtrl = m_pMtrlList[iMtrl];
		LoadMaterial(pMtrl);
	}

	for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++) // 서브 머테리얼
	{
		FbxNode* pNode = m_pFbxNodeList[iNode];
		KMesh* pMesh = new KMesh;
		m_pMeshList.push_back(pMesh);
		ParseNode(pNode, pMesh);

	}

	for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++) // 서브텍스처
	{
		KMesh* pMesh = m_pMeshList[iMesh];
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_pMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
			{
				KMesh* pSubMesh = m_pMeshList[iMesh]->m_pSubMesh[iSubMesh];
				pSubMesh->Create(pContext, L"../../Data/Shader/FbxShader.txt", L"../../Data/Shader/FbxShader.txt");
			}
		}
		else
		{
			pMesh->Create(pContext, L"../../Data/Shader/FbxShader.txt", L"../../Data/Shader/FbxShader.txt");
		}
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
		KMtrl* pMtrl = nullptr;
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (pMesh->m_pSubMesh.size() > 0) // Sub
		{
			for (int iSub = 0; iSub < pMesh->m_pSubMesh.size(); iSub++)
			{
				KMtrl* pSubMtrl =
					m_pMtrlList[pMesh->GetRef()]->m_pSubMtrl[iSub];
				pContext->PSSetSamplers(0, 1, &pSubMtrl->m_Texture.m_pSampler);
				pContext->PSSetShaderResources(1, 1, &pSubMtrl->m_Texture.m_pTextureSRV);
				pMesh->m_pSubMesh[iSub]->SetMatrix(&pMesh->m_matWorld, &m_kbData.matView, &m_kbData.matProj);
				pMesh->m_pSubMesh[iSub]->Render();
			}
		}
		else 
		{
			if (pMesh->GetRef() >= 0)
			{
				pMtrl = m_pMtrlList[pMesh->GetRef()];
			}			
			if (pMtrl != nullptr)
			{
				pContext->PSSetSamplers(0, 1, &pMtrl->m_Texture.m_pSampler);
				pContext->PSSetShaderResources(1, 1, &pMtrl->m_Texture.m_pTextureSRV);
			}
			pMesh->SetMatrix(&pMesh->m_matWorld, &m_kbData.matView, &m_kbData.matProj);
			pMesh->Render();
		}
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
}

void KFbxObj::LoadMaterial(KMtrl* pMtrl)
{
	_ASSERT(pMtrl != nullptr);
	if (pMtrl->m_pSubMtrl.size() > 0)
	{
		for (int iSub = 0; iSub < pMtrl->m_pSubMtrl.size(); iSub++)
		{
			KMtrl* pSubMtrl = pMtrl->m_pSubMtrl[iSub];
			FbxSurfaceMaterial* pFbxMaterial = pSubMtrl->m_pFbxMtrl;
			FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (prop.IsValid())
			{
				
				int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
				for (int iTex = 0; iTex < iTexCount; iTex++)
				{
					FbxTexture* pTex = prop.GetSrcObject<FbxFileTexture>(iTex);
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
						if (_stricmp(Ext, ".tga") == 0)
						{
							szFileName += ".dds";
						}
						else 
						{
							szFileName += Ext;
						}
					}
					pSubMtrl->m_Texture.m_szFileName = KBASE::g_szDataPath;
					pSubMtrl->m_Texture.m_szFileName += L"Object/";
					pSubMtrl->m_Texture.m_szFileName += KBASE::mtw(szFileName);
					pSubMtrl->m_Texture.LoadTexture(pSubMtrl->m_Texture.m_szFileName);
				}
			}
		}
	}
	else
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
					if (_stricmp(Ext, ".tga") == 0)
					{
						szFileName += ".dds";
					}
					else
					{
						szFileName += Ext;
					}
				}
				pMtrl->m_Texture.m_szFileName = KBASE::g_szDataPath;
				pMtrl->m_Texture.m_szFileName += L"Object/";
				pMtrl->m_Texture.m_szFileName += KBASE::mtw(szFileName);
				pMtrl->m_Texture.LoadTexture(pMtrl->m_Texture.m_szFileName);
			}
		}
	}
}

void KFbxObj::PreProcess(FbxNode* pNode)
{
	int iNumFbxMtrl = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	
	if (iNumFbxMtrl > 1) //서브존재
	{
		KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
		for (int iSub = 0; iSub < iNumFbxMtrl; iSub++)
		{
			FbxSurfaceMaterial* pFbxSubMtrl = pNode->GetMaterial(iSub);
			_ASSERT(pFbxSubMtrl != nullptr);
			KMtrl* pSubMtrl = new KMtrl(pNode, pFbxMaterial);
			pMtrl->m_pSubMtrl.push_back(pSubMtrl);
		}
		m_pMtrlList.push_back(pMtrl);

	}
	else // 서브없음
	{
		if (pFbxMaterial != nullptr)
		{
			KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
			m_pMtrlList.push_back(pMtrl);
		}
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
		pMesh->m_iNumLayer = pFbxMesh->GetLayerCount();
		pMesh->m_LayerList.resize(pMesh->m_iNumLayer);
		// 정점성분 레이어 리스트
		for (int iLayer = 0; iLayer < pMesh->m_iNumLayer; iLayer++)
		{
			FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
			if (pLayer->GetVertexColors() != nullptr) 
			{
				pMesh->m_LayerList[iLayer].pColor = pLayer->GetVertexColors();
			}
			if (pLayer->GetNormals() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pNormal = pLayer->GetNormals();
			}
			if (pLayer->GetUVs() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pUV = pLayer->GetUVs();
			}
			if (pLayer->GetMaterials() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pMaterial = pLayer->GetMaterials();
			}
		}
		FbxAMatrix matGeom;//지오메트리
		{
			FbxVector4 rot;
			rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
			FbxVector4 trans;
			trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
			FbxVector4 scale
				= pNode->GetGeometricScaling(FbxNode::eSourcePivot);
			matGeom.SetT(trans);
			matGeom.SetS(scale);
			matGeom.SetR(rot);

		}
		FbxMatrix matA = matGeom;
		pMesh->m_matWorld = DxConvertMatrix(ConvertMatrix(matA));
		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// 정점리스트 주소
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		int iBasePlayIndex = 0;
		int iNumFbxMaterial = pNode->GetMaterialCount();
		if (iNumFbxMaterial > 1)
		{
			pMesh->m_pSubMesh.resize(iNumFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				pMesh->m_pSubMesh[iSub] = new KMesh;
			}
		}
		FbxLayerElementMaterial* fbxSubMaterial =
			pMesh->m_LayerList[0].pMaterial;
		FbxLayerElementUV* VertexUVList =
			pMesh->m_LayerList[0].pUV;
		FbxLayerElementVertexColor* VertexColorList =
			pMesh->m_LayerList[0].pColor;
		FbxLayerElementNormal* VertexNormalList =
			pMesh->m_LayerList[0].pNormal;
		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			int iSubMtrlIndex = 0;
			if (fbxSubMaterial != nullptr)
			{
				switch (fbxSubMaterial->GetMappingMode())
				{
				case FbxLayerElement::eByPolygon:
				{
					switch (fbxSubMaterial->GetReferenceMode())
					{
					case FbxLayerElement::eDirect:
					{
						iSubMtrlIndex = iPoly;
					}break;
					case FbxLayerElement::eIndex:
					case FbxLayerElement::eIndexToDirect:
					{
						iSubMtrlIndex =
							fbxSubMaterial->GetIndexArray().GetAt(iPoly);
					}break;
					}
				}break;
				default:
				{
					iSubMtrlIndex = 0;
				}break;
				}
			}
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
					//FbxVector4 vPos = matGeom.MultT(pos);
					vertex.pos.x = pos.mData[0];
					vertex.pos.y = pos.mData[2];
					vertex.pos.z = pos.mData[1];
					if (VertexUVList != nullptr)
					{
						FbxVector2 uv = m_kMesh.ReadTextureCoord(
							pFbxMesh, 1, VertexUVList,
							iCornerIndex[iIndex], u[iIndex]);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f - uv.mData[1];
					}
					if (VertexColorList != nullptr)
					{
						int iColorIndex[3];
						iColorIndex[0] = iBasePlayIndex + 0;
						iColorIndex[1] = iBasePlayIndex + iTriangle + 2;
						iColorIndex[2] = iBasePlayIndex + iTriangle + 1;

						FbxColor color = m_kMesh.ReadColor(
							pFbxMesh, 1, VertexColorList,
							iCornerIndex[iIndex], iColorIndex[iIndex]);
						vertex.color.x = color.mRed;
						vertex.color.y = color.mGreen;
						vertex.color.z = color.mBlue;
						vertex.color.w = 1.0f;

					}
					if (VertexNormalList != nullptr)
					{
						int iNormalIndex[3];
						iNormalIndex[0] = iBasePlayIndex + 0;
						iNormalIndex[1] = iBasePlayIndex + iTriangle + 2;
						iNormalIndex[2] = iBasePlayIndex + iTriangle + 1;
						FbxVector4 normal = m_kMesh.ReadNormal(
							pFbxMesh, 1, VertexNormalList,
							iCornerIndex[iIndex], iNormalIndex[iIndex]);
						vertex.normal.x = normal.mData[0];
						vertex.normal.y = normal.mData[2];
						vertex.normal.z = normal.mData[1];
					}
					if (iNumFbxMaterial > 1)
					{
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_VertexList.push_back(vertex);
					}
					else
					{
						pMesh->m_VertexList.push_back(vertex);
					}
				}
			}
			iBasePlayIndex += iPolySize;
		}
	}
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
	return true;
}

TMatrix KFbxObj::DxConvertMatrix(TMatrix mat)
{
	TMatrix tmp;
	tmp._11 = mat._11; tmp._12 = mat._13; tmp._13 = mat._12;
	tmp._21 = mat._31; tmp._22 = mat._33; tmp._23 = mat._32;
	tmp._31 = mat._21; tmp._32 = mat._23; tmp._33 = mat._22;
	tmp._41 = mat._41; tmp._42 = mat._43; tmp._43 = mat._42;
	tmp._14 = tmp._24 = tmp._34 = 0.0f;
	tmp._44 = 1.0f;
	return tmp;
}

TMatrix KFbxObj::ConvertMatrix(FbxMatrix& mat)
{
	TMatrix tmp;
	float* pMatArray = reinterpret_cast<float*>(&tmp);
	double* pSrcArray = reinterpret_cast<double*>(&mat);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return tmp;
}

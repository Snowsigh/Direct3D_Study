#include "KFbxObj.h"
#include <algorithm>
bool KFbxObj::LoadObject(std::string filename, LPCWSTR shadername, ID3D11DeviceContext* pContext)
{
	m_pContext = pContext;
    m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
    m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
    bool bRet = m_pFbxImporter->Initialize(filename.c_str());
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene); // 기저축 설정


    FbxNode* m_pRootNode = m_pFbxScene->GetRootNode();
	m_pFbxNodeList.push_back(m_pRootNode);
	TMatrix matWorld;
	m_kAni.m_matBindPoseList.push_back(matWorld);
    PreProcess(m_pRootNode);

	for (int iMtrl = 0; iMtrl < m_pMtrlList.size(); iMtrl++) // 머테리얼
	{
		KMtrl* pMtrl = m_pMtrlList[iMtrl];
		LoadMaterial(pMtrl);
	}

	m_kAni.SetAni(m_pFbxScene, m_pFbxNodeList);
	ParseNode(m_pRootNode, nullptr);
	m_kAni.ParseAnimationNode(m_pMeshList);

	for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++) // 서브텍스처
	{
		KMesh* pMesh = m_pMeshList[iMesh];
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_pMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
			{
				KMesh* pSubMesh = m_pMeshList[iMesh]->m_pSubMesh[iSubMesh];
				pSubMesh->Create(pContext, shadername, shadername);
			}
		}
		else
		{
			pMesh->Create(pContext, shadername, shadername);
		}
	}
	m_pFbxScene->Destroy();
	m_pFbxImporter->Destroy();
	m_pFbxManager->Destroy();
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

bool KFbxObj::Render()
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		KMesh* pMesh = m_pMeshList[iObj];
		if (pMesh->m_ClassType != CLASS_GEOM) continue;

		for (int iBone = 0; iBone < pMesh->m_iBoneList.size(); iBone++)
		{
			int iIndex = pMesh->m_iBoneList[iBone];
			pMesh->m_matAnimMatrix.matAnimation[iBone] =
				m_matAnimMatrix.matAnimation[iIndex];
			pMesh->m_matAnimMatrix.matAnimation[iBone] =
				pMesh->m_matAnimMatrix.matAnimation[iBone].Transpose();
		}
		
		KMtrl* pMtrl = nullptr;
		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (pMesh->m_pSubMesh.size() > 0) // Sub
		{
			for (int iSub = 0; iSub < pMesh->m_pSubMesh.size(); iSub++)
			{
				if (pMesh->m_pSubMesh[iSub]->m_VertexList.size() <= 0) continue;
				
				KMtrl* pSubMtrl =
					m_pMtrlList[pMesh->GetRef()]->m_pSubMtrl[iSub];

				m_pContext->PSSetSamplers(0, 1, &pSubMtrl->m_Texture.m_pSampler);
				m_pContext->PSSetShaderResources(1, 1, &pSubMtrl->m_Texture.m_pTextureSRV);
				pMesh->m_pSubMesh[iSub]->SetMatrix(&pMesh->m_AnimationTrack[m_kAni.m_iAnimIndex], &m_kbData.matView, &m_kbData.matProj);
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
				m_pContext->PSSetSamplers(0, 1, &pMtrl->m_Texture.m_pSampler);
				m_pContext->PSSetShaderResources(1, 1, &pMtrl->m_Texture.m_pTextureSRV);
			}
			pMesh->SetMatrix(&pMesh->m_AnimationTrack[m_kAni.m_iAnimIndex], &m_kbData.matView, &m_kbData.matProj);
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
	if (pNode->GetCamera() || pNode->GetLight())
	{
		return;
	}
	int iNumFbxMaterial = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	if (GetRootMtrl(pFbxMaterial) == -1)
	{
		if (iNumFbxMaterial > 1)
		{
			KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				FbxSurfaceMaterial* pFbxSubMaterial = pNode->GetMaterial(iSub);
				_ASSERT(pFbxSubMaterial != nullptr);
				KMtrl* pSubMtrl = new KMtrl(pNode, pFbxSubMaterial); //pFbxMaterial <- 이걸로 써놓음
				pMtrl->m_pSubMtrl.push_back(pSubMtrl);
			}
			m_pMtrlList.push_back(pMtrl);
		}
		else
		{
			if (pFbxMaterial != nullptr)
			{
				KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
				m_pMtrlList.push_back(pMtrl);
			}
		}
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		FbxNodeAttribute::EType type =
			pChildNode->GetNodeAttribute()->GetAttributeType();
		if (/*type == FbxNodeAttribute::eMesh ||
			type == FbxNodeAttribute::eSkeleton*/
			pChildNode->GetNodeAttribute() != nullptr)
		{
			m_pFbxNodeList.push_back(pChildNode);
			m_kAni.m_matBindPoseList.push_back(TMatrix());
		}
		PreProcess(pChildNode);
	}
}

void KFbxObj::ParseNode(FbxNode* pNode, KMesh* pParentMesh)
{
	if (pNode->GetCamera() || pNode->GetLight())
	{
		return;
	}
	KMesh* pMesh = new KMesh;
	pMesh->m_pFbxNode = pNode;
	pMesh->m_szName = KBASE::mtw(pNode->GetName());
	TMatrix matParent;
	if (pParentMesh != nullptr)
	{
		pMesh->m_szParentName = pParentMesh->m_szName;
		matParent = pParentMesh->m_matWorld;
	}
	pMesh->m_pParent = pParentMesh;
	pMesh->m_matWorld = ParseTransform(pNode, matParent);

	

	if (pNode->GetMesh())
	{
		ParseMesh(pNode, pMesh);
		pMesh->m_ClassType = CLASS_GEOM;
	}
	else
	{
		pMesh->m_ClassType = CLASS_BONE;
	}
	m_pMeshList.push_back(pMesh);

	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		ParseNode(pChildNode, pMesh);
	}

}

bool KFbxObj::Release()
{
	
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		m_pMeshList[iObj]->Release();
		if (m_pMeshList[iObj])delete m_pMeshList[iObj];
	}

	for (int iObj = 0; iObj < m_pMtrlList.size(); iObj++)
	{
		m_pMtrlList[iObj]->Release();
		if(m_pMtrlList[iObj])delete m_pMtrlList[iObj];
	}
	return true;
}


TMatrix   KFbxObj::ParseTransform(FbxNode* pNode, TMatrix& matParent)
{
	//// TODO : 월드행렬
	FbxVector4 rotLcl = pNode->LclRotation.Get();
	FbxVector4 transLcl = pNode->LclTranslation.Get();
	FbxVector4 scaleLcl = pNode->LclScaling.Get();
	FbxMatrix matTransform(transLcl, rotLcl, scaleLcl);
	TMatrix matLocal = KBASE::DxConvertMatrix(KBASE::ConvertMatrix(matTransform));
	TMatrix matWorld = matLocal * matParent;
	return matWorld;
}

void	KFbxObj::ParseMesh(FbxNode* pNode, KMesh* pMesh)
{

	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	pMesh->SetRef(GetRootMtrl(pFbxMaterial));

	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	std::vector< std::string> fbxFileTexList;
	if (pFbxMesh != nullptr)
	{

		int iNumCP = pFbxMesh->GetControlPointsCount();
		// 스키닝 오브젝트 여부?
		// pFbxMesh에 영향을 미치는 행렬(노드)에 전체 개수?
		// 행렬[0]=FbxNode ~ 행렬[3] = 4개
		// 정점[0]->인덱스[1]		// 		
		KSkinData skindata;
		bool bSkinnedMesh = m_kAni.ParseMeshSkinning(pFbxMesh, pMesh, &skindata);
		_ASSERT(skindata.m_VertexList.size() == iNumCP);
		if (bSkinnedMesh)
		{
			for (int i = 0; i < skindata.m_VertexList.size(); i++)
			{
				std::vector<std::pair<float, int>> list;
				for (int j = 0; j <
					skindata.m_VertexList[i].m_IndexList.size();
					j++)
				{
					list.push_back(std::make_pair(
						skindata.m_VertexList[i].m_WegihtList[j],
						skindata.m_VertexList[i].m_IndexList[j]));
				}
				std::sort(list.begin(), list.end(), KBASE::Compare);
				for (int k = 0; k < list.size(); k++)
				{
					skindata.m_VertexList[i].m_WegihtList[k] = list[k].first;
					skindata.m_VertexList[i].m_IndexList[k] = list[k].second;
				}
			}
		}

		pMesh->m_iNumLayer = pFbxMesh->GetLayerCount();
		pMesh->m_LayerList.resize(pMesh->m_iNumLayer);

		// todo : 정점성분 레이어 리스트
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
		// TODO : 월드행렬
		FbxAMatrix matGeom;
		FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		matGeom.SetT(trans);
		matGeom.SetR(rot);
		matGeom.SetS(scale);

		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// 정점리스트 주소
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
		int iBasePolyIndex = 0;
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

		KMesh m_kMesh; //Mesh 클래스의 함수 계산을 가져오기 위한 지역 클래스

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
			if (iSubMtrlIndex < 0 || iSubMtrlIndex >= iNumFbxMaterial)
			{
				int kk = 0;
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
				

				for (int iIndex = 0;
					iIndex < 3;
					iIndex++)
				{
					PNCT_VERTEX vertex;
					PNCTIW_VERTEX iwVertex;											//애니메이션 수정사항 21.10.25
					ZeroMemory(&iwVertex, sizeof(PNCTIW_VERTEX));
					FbxVector4 pos = pVertexPositions[iCornerIndex[iIndex]];
					FbxVector4 vPos = matGeom.MultT(pos);
					vertex.pos.x = (float)vPos.mData[0];
					vertex.pos.y = (float)vPos.mData[2];
					vertex.pos.z = (float)vPos.mData[1];
					if (VertexUVList != nullptr)
					{
						// UV 인덱스
						int uvIndex[3];
						uvIndex[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
						uvIndex[1] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
						uvIndex[2] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);
						FbxVector2 uv = m_kMesh.ReadTextureCoord(
							pFbxMesh, 1, VertexUVList,
							iCornerIndex[iIndex], uvIndex[iIndex]);
						vertex.tex.x = (float)(uv.mData[0]);
						vertex.tex.y = (float)(1.0f - uv.mData[1]);
					}
					if (VertexColorList != nullptr)
					{
						int iColorIndex[3];
						iColorIndex[0] = iBasePolyIndex + 0;
						iColorIndex[1] = iBasePolyIndex + iTriangle + 2;
						iColorIndex[2] = iBasePolyIndex + iTriangle + 1;

						FbxColor color = m_kMesh.ReadColor(
							pFbxMesh, 1, VertexColorList,
							iCornerIndex[iIndex], iColorIndex[iIndex]);
						vertex.color.x = (float)color.mRed;
						vertex.color.y = (float)color.mGreen;
						vertex.color.z = (float)color.mBlue;
						vertex.color.w = 1.0f;

						

					}
					if (VertexNormalList != nullptr)
					{
						int iNormalIndex[3];
						iNormalIndex[0] = iBasePolyIndex + 0;
						iNormalIndex[1] = iBasePolyIndex + iTriangle + 2;
						iNormalIndex[2] = iBasePolyIndex + iTriangle + 1;
						FbxVector4 normal = m_kMesh.ReadNormal(
							pFbxMesh, 1, VertexNormalList,
							iCornerIndex[iIndex], iNormalIndex[iIndex]);
						vertex.normal.x = (float)normal.mData[0];
						vertex.normal.y = (float)normal.mData[2];
						vertex.normal.z = (float)normal.mData[1];
					}
					// 인덱스 및 가중치 저장
					int iRealIndex = iCornerIndex[iIndex];
					if (bSkinnedMesh)
					{
						int iNum =
							skindata.m_VertexList[iRealIndex].m_IndexList.size();
						for (int i = 0; i < min(iNum, 4); i++)
						{
							iwVertex.index[i] =
								skindata.m_VertexList[iRealIndex].m_IndexList[i];
							iwVertex.weight[i] =
								skindata.m_VertexList[iRealIndex].m_WegihtList[i];
						}
					}
					// 비 스키닝 오브젝트를 -> 스키닝화 처리
					else
					{
						iwVertex.index[0] = 0;
						iwVertex.weight[0] = 1.0f;
						for (int i = 0; i < 4; i++)
						{
							iwVertex.index[i] = 0;
							iwVertex.weight[i] = 0.0f;
						}
					}
					if (iNumFbxMaterial > 1)
					{
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_VertexList.push_back(vertex);
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_WeightList.push_back(iwVertex);
					}
					else
					{
						pMesh->m_VertexList.push_back(vertex);
						pMesh->m_WeightList.push_back(iwVertex);
					}
				}
			}
			iBasePolyIndex += iPolySize;
		}
	}
}

bool KFbxObj::Frame()
{
	m_kAni.m_fElpaseTime += g_fSPF * 1.0f;
	m_kAni.m_iAnimIndex = (int)(m_kAni.m_fElpaseTime * 30.0f);
	if (m_kAni.m_fEndTime < m_kAni.m_fElpaseTime)
	{
		m_kAni.m_iAnimIndex = 0;
		m_kAni.m_fElpaseTime = 0;
	}
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		KMesh* pMesh = m_pMeshList[iObj];
		m_matAnimMatrix.matAnimation[iObj] =
			m_kAni.m_matBindPoseList[iObj] *
			pMesh->m_AnimationTrack[m_kAni.m_iAnimIndex];
	}
	return true;
}

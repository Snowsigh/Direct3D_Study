#include "KQuadTree.h"
#include <iterator>

bool KQuadTree::GetCamera(KCamera* pCamera)
{
	m_pCamera = pCamera;
	return false;
}
// 0           2            4
// 5           7             
// 10          12           14
// 15           17 
// 20          22           24 		
template <typename OutputIterator>
void KQuadTree::Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first)
{
	size_t start, stop, n = text.length();

	for (start = text.find_first_not_of(delimiters); 0 <= start && start < n;
		start = text.find_first_not_of(delimiters, stop + 1))
	{
		stop = text.find_first_of(delimiters, start);
		if (stop < 0 || stop > n)
		{
			stop = n;
		}
		*first++ = text.substr(start, stop - start);
	}
}
bool  KQuadTree::LoadObject(std::wstring filename)
{
	FILE* fp = nullptr;
	_tfopen_s(&fp, filename.c_str(), _T("rt"));
	if (fp == NULL)
	{
		return false;
	}

	TCHAR data[256] = { 0, };

	TCHAR buffer[256] = { 0, };
	int iVersion = 0;
	_fgetts(buffer, 256, fp);
	TCHAR tmp[256] = { 0, };
	_stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

	int iNumPatch = 0;
	_fgetts(buffer, 256, fp);
	_stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iNumPatch);

	int index = 0;

	for (int iLod = 0; iLod < iNumPatch; iLod++)
	{
		KLodPatch lod;
		lod.iLodLevel = iLod;
		for (int iCode = 0; iCode < 16; iCode++)
		{
			std::vector<std::wstring>	ListTokens;
			_fgetts(buffer, 256, fp);
			_stscanf_s(buffer, _T("%d %s"), &index, data, _countof(data));

			std::wstring sentence = data;
			Tokenize(sentence, L",", std::back_inserter(ListTokens));
			int iMaxCnt = (int)ListTokens.size();
			lod.IndexList[iCode].resize(iMaxCnt);
			for (int i = 0; i < iMaxCnt; i++)
			{
				lod.IndexList[iCode][i] = (DWORD)(_tstoi(ListTokens[i].c_str()));
			}
		}
		m_LodPatchlist.push_back(lod);
		
		_fgetts(buffer, 256, fp);
	}
	for (int iLod = 0; iLod < iNumPatch; iLod++)
	{
		for (int iCode = 0; iCode < 16; iCode++)
		{
			CreateIndexBuffer(m_LodPatchlist[iLod], iCode);
		}
	}
	fclose(fp);
	return true;
}
HRESULT KQuadTree::CreateIndexBuffer(KLodPatch& patch, int iCode)
{
	patch.IndexBufferList[iCode] = nullptr;
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * patch.IndexList[iCode].size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &patch.IndexList[iCode].at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &patch.IndexBufferList[iCode]);
	if (FAILED(hr)) return hr;
	return hr;
}
bool KQuadTree::UpdateIndexList(KNode* pNode)
{
	int iNumCols = m_pMap->m_mapInfo.m_iNumCol;
	int iStartRow = pNode->m_CornerList[0] / iNumCols;
	int iEndRow = pNode->m_CornerList[2] / iNumCols;
	int iStartCol = pNode->m_CornerList[0] % iNumCols;
	int iEndCol = pNode->m_CornerList[1] % iNumCols;

	int iNumColCell = iEndCol - iStartCol;
	int iNumRowCell = iEndRow - iStartRow;
	m_Indexlist.resize(iNumColCell * iNumRowCell * 2 * 3);

	int iIndex = 0;
	for (int iRow = 0; iRow < iNumRowCell; iRow++)
	{
		for (int iCol = 0; iCol < iNumColCell; iCol++)
		{
			int iCurrentIndex = iRow * (iNumColCell + 1) + iCol;
			int iNextRow = (iRow + 1) * (iNumColCell + 1) + iCol;
			m_Indexlist[iIndex + 0] = iCurrentIndex;
			m_Indexlist[iIndex + 1] = iCurrentIndex + 1;
			m_Indexlist[iIndex + 2] = iNextRow;

			m_Indexlist[iIndex + 3] = m_Indexlist[iIndex + 2];
			m_Indexlist[iIndex + 4] = m_Indexlist[iIndex + 1];
			m_Indexlist[iIndex + 5] = iNextRow + 1;
			iIndex += 6;
		}
	}
	if (m_Indexlist.size() > 0) return true;
	return false;
}
HRESULT KQuadTree::CreateIndexBuffer(KNode* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_Indexlist.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_Indexlist.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
	if (FAILED(hr)) return hr;
	return hr;
}
bool KQuadTree::UpdateVertexList(KNode* pNode)
{
	int iNumCols = m_pMap->m_mapInfo.m_iNumCol;
	int iStartRow = pNode->m_CornerList[0] / iNumCols;
	int iEndRow = pNode->m_CornerList[2] / iNumCols;
	int iStartCol = pNode->m_CornerList[0] % iNumCols;
	int iEndCol = pNode->m_CornerList[1] % iNumCols;

	int iNumColCell = iEndCol - iStartCol;
	int iNumRowCell = iEndRow - iStartRow;
	pNode->m_pVertexList.resize((iEndCol - iStartCol + 1) * (iEndRow - iStartRow + 1));

	int iIndex = 0;
	for (int iRow = iStartRow; iRow <= iEndRow; iRow++)
	{
		for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
		{
			int iCurrentIndex = iRow * iNumCols + iCol;
			pNode->m_pVertexList[iIndex++] = m_pMap->m_VertexList[iCurrentIndex];
		}
	}
	if (pNode->m_pVertexList.size() > 0) return true;
	return false;
}
HRESULT KQuadTree::CreateVertexBuffer(KNode* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(SimpleVertex) * pNode->m_pVertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &pNode->m_pVertexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &pNode->m_pVertexBuffer);
	if (FAILED(hr)) return hr;
	return hr;
}
bool	KQuadTree::Render(ID3D11DeviceContext* pContext)
{
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		int iLodLevel = 0;
		float fDistance = (m_pLeafList[iNode]->m_vCenter - m_pCamera->m_vCameraPos).Length();
		if (fDistance < 30.0f)
		{
			m_pLeafList[iNode]->m_iLodLevel = 2;
		}
		else if (fDistance < 60.0f)
		{
			m_pLeafList[iNode]->m_iLodLevel = 1;
		}
		else
			m_pLeafList[iNode]->m_iLodLevel = 0;
	}

	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		int iRenderCode = 0;
		// 동서남북
		if (m_pLeafList[iNode]->m_NeighborList[0] &&
			m_pLeafList[iNode]->m_iLodLevel < m_pLeafList[iNode]->m_NeighborList[0]->m_iLodLevel)
		{
			iRenderCode += 2;
		}
		if (m_pLeafList[iNode]->m_NeighborList[1] &&
			m_pLeafList[iNode]->m_iLodLevel < m_pLeafList[iNode]->m_NeighborList[1]->m_iLodLevel)
		{
			iRenderCode += 8;
		}
		if (m_pLeafList[iNode]->m_NeighborList[2] &&
			m_pLeafList[iNode]->m_iLodLevel < m_pLeafList[iNode]->m_NeighborList[2]->m_iLodLevel)
		{
			iRenderCode += 4;
		}
		if (m_pLeafList[iNode]->m_NeighborList[3] &&
			m_pLeafList[iNode]->m_iLodLevel < m_pLeafList[iNode]->m_NeighborList[3]->m_iLodLevel)
		{
			iRenderCode += 1;
		}

		UINT iNumIndex = 0;
		ID3D11Buffer* pRenderBuffer = nullptr;
		UINT iLodLevel = m_pLeafList[iNode]->m_iLodLevel;
		if (m_pLeafList[iNode]->m_iLodLevel == 0)
		{
			iNumIndex = m_LodPatchlist[iLodLevel].IndexList[iRenderCode].size();
			pRenderBuffer = m_LodPatchlist[iLodLevel].IndexBufferList[iRenderCode];
		}
		else if (m_pLeafList[iNode]->m_iLodLevel == 1)
		{
			iNumIndex = m_LodPatchlist[iLodLevel].IndexList[iRenderCode].size();
			pRenderBuffer = m_LodPatchlist[iLodLevel].IndexBufferList[iRenderCode];
		}
		else
		{
			iNumIndex = m_Indexlist.size();
			pRenderBuffer = m_pIndexBuffer;
		}

		m_pMap->PreRender();
		UINT pStrides = sizeof(SimpleVertex);
		UINT pOffsets = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pLeafList[iNode]->m_pVertexBuffer, &pStrides, &pOffsets);
		pContext->IASetIndexBuffer(pRenderBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_pMap->PostRender(iNumIndex);
	}
	return true;
}
bool	KQuadTree::Frame()
{
	//player.sumForces.x = 10.0f;
	//player.sumForces.y = 0.0f;
	//// F = MA, A =  F / M
	//player.Acceleration.x = player.sumForces.x / player.mass;
	//player.Acceleration.y = player.sumForces.y / player.mass;
	//player.Velocity += player.Acceleration * time;
	//player.pos += player.Velocity * time;
	//TNode* pFind = FindPlayerNode(player.pos);
	//if (pFind != nullptr)
	//{
	//	cout << pFind->m_iIndex << " ";
	//}
	return true;
}
void    KQuadTree::Build(KMap* pMap, KCamera* pCamera)
{
	
	GetCamera(pCamera);
	m_pMap = pMap;
	m_iNumCol = m_pMap->m_mapInfo.m_iNumCol;
	m_iNumRow = m_pMap->m_mapInfo.m_iNumRow;
	m_pRootNode = CreateNode(nullptr, 0, m_iNumCol - 1,
		(m_iNumRow - 1) * m_iNumCol, m_iNumRow * m_iNumCol - 1);
	Buildtree(m_pRootNode);
	SetNeighborNode();
	// lod patch (전체 가로 개수(9), 리프노드 깊이(1))
	m_iNumCell = (m_iNumCol - 1) / pow(2.0f, m_iMaxDepth);
	m_iNumPatch = (log(m_iNumCell) / log(2.0f));
	/*if (m_iNumPatch > 0)
	{
		m_LodPatchlist.resize(m_iNumPatch);   << ?? 왜안되지? 
	}*/
	//// 공유 정점버퍼
	if (UpdateIndexList(m_pLeafList[0]))
	{
		CreateIndexBuffer(m_pLeafList[0]);
	}
}
bool    KQuadTree::Init()
{
	return true;
}
bool  KQuadTree::SubDivide(KNode* pNode)
{
	if ((pNode->m_CornerList[1] - pNode->m_CornerList[0]) > 4)
	{
		return true;
	}
	return false;
}
void KQuadTree::Buildtree(KNode* pNode)
{
	if (SubDivide(pNode))
	{
		// 0           2            4
		//                         
		// 10          12           14
		// 
		// 20          22           24 		
		int center = (pNode->m_CornerList[3] + pNode->m_CornerList[0]) / 2;
		int mt = (pNode->m_CornerList[0] + pNode->m_CornerList[1]) / 2;
		int ml = (pNode->m_CornerList[2] + pNode->m_CornerList[0]) / 2;
		int mr = (pNode->m_CornerList[3] + pNode->m_CornerList[1]) / 2;
		int mb = (pNode->m_CornerList[2] + pNode->m_CornerList[3]) / 2;

		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_CornerList[0],
			mt,
			ml,
			center);
		Buildtree(pNode->m_pChild[0]);

		pNode->m_pChild[1] = CreateNode(pNode, mt,
			pNode->m_CornerList[1],
			center,
			mr);
		Buildtree(pNode->m_pChild[1]);

		pNode->m_pChild[2] = CreateNode(pNode, ml,
			center,
			pNode->m_CornerList[2],
			mb);
		Buildtree(pNode->m_pChild[2]);

		pNode->m_pChild[3] = CreateNode(pNode, center,
			mr,
			mb,
			pNode->m_CornerList[3]);
		Buildtree(pNode->m_pChild[3]);
	}
	else
	{
		pNode->m_bLeaf = true;
		if (m_iMaxDepth < pNode->m_iDepth)
		{
			m_iMaxDepth = pNode->m_iDepth;
		}
		// 공유 인덱스버퍼용(정점버퍼 리프노드 당)
		if (UpdateVertexList(pNode))
		{
			CreateVertexBuffer(pNode);
		}
		m_pLeafList.insert(std::make_pair(pNode->m_iIndex, pNode));
	}
}
KNode* KQuadTree::FindNode(KNode* pNode, TVector2 pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr &&
				pNode->m_pChild[iNode]->IsRect(pos))
			{
				m_Queue.push(pNode->m_pChild[iNode]);
				break;
			}
		}
		if (m_Queue.empty())break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);
	return pNode;
}
bool    KQuadTree::AddObject(TVector2 pos)
{
	KNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}
KNode* KQuadTree::FindPlayerNode(TVector2 pos)
{
	KNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		return pFindNode;
	}
	return nullptr;
}
bool KQuadTree::Release()
{
	for (int iPatch = 0; iPatch < m_LodPatchlist.size(); iPatch++)
	{
		m_LodPatchlist[iPatch].Release();
	}
	if (m_pIndexBuffer)m_pIndexBuffer->Release();
	delete m_pRootNode;
	m_pRootNode = nullptr;
	return true;
}
void KQuadTree::SetNeighborNode()
{
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		auto iter = m_pLeafList.find(iNode);
		_ASSERT(iter != m_pLeafList.end());
		KNode* pNode = iter->second;
		DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->m_iDepth);
		DWORD dwNeighborCol, dwNeighborRow;

		if (pNode->m_Element.y > 0)  // 상
		{
			dwNeighborCol = pNode->m_Element.x;
			dwNeighborRow = (pNode->m_Element.y - 1) * dwNumPatchCount;
			auto iter = m_pLeafList.find(dwNeighborRow + dwNeighborCol);
			_ASSERT(iter != m_pLeafList.end());
			pNode->m_NeighborList[3] = iter->second;;
		}
		if (pNode->m_Element.y < dwNumPatchCount - 1) // 하
		{
			dwNeighborCol = pNode->m_Element.x;
			dwNeighborRow = (pNode->m_Element.y + 1) * dwNumPatchCount;
			auto iter = m_pLeafList.find(dwNeighborRow + dwNeighborCol);
			_ASSERT(iter != m_pLeafList.end());
			pNode->m_NeighborList[2] = iter->second;;
		}
		if (pNode->m_Element.x > 0) // 좌
		{
			dwNeighborCol = pNode->m_Element.x - 1;
			dwNeighborRow = pNode->m_Element.y * dwNumPatchCount;
			auto iter = m_pLeafList.find(dwNeighborRow + dwNeighborCol);
			_ASSERT(iter != m_pLeafList.end());
			pNode->m_NeighborList[1] = iter->second;;
		}
		if (pNode->m_Element.x < dwNumPatchCount - 1) // 우
		{
			dwNeighborCol = pNode->m_Element.x + 1;
			dwNeighborRow = pNode->m_Element.y * dwNumPatchCount;
			auto iter = m_pLeafList.find(dwNeighborRow + dwNeighborCol);
			_ASSERT(iter != m_pLeafList.end());
			pNode->m_NeighborList[0] = iter->second;;
		}
	}
}
KNode* KQuadTree::CreateNode(KNode* pParent, float x, float y, float w, float h)
{
	KNode* pNode = new KNode(x, y, w, h);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}
	// 쿼드트리의 성분 저장(ldiv함수의 결과와 동일하여 대체되었다.)
	//pNode->m_Element.x = (pNode->m_CornerList[0] % m_iNumCol) / (pNode->m_CornerList[1] - pNode->m_CornerList[0]);
	//pNode->m_Element.y = pNode->m_CornerList[0] / (pNode->m_CornerList[2] - pNode->m_CornerList[0]);
	ldiv_t divValue = ldiv(pNode->m_CornerList[0], m_iNumCol);
	pNode->m_Element.x = divValue.rem / (pNode->m_CornerList[1] - pNode->m_CornerList[0]); // 나머지-> X
	pNode->m_Element.y = divValue.quot / (pNode->m_CornerList[1] - pNode->m_CornerList[0]);//몫 -> Y

	DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->m_iDepth);
	pNode->m_iIndex = pNode->m_Element.y * dwNumPatchCount + pNode->m_Element.x;

	TVector3 vLT = m_pMap->m_VertexList[pNode->m_CornerList[0]].pos;
	TVector3 vRT = m_pMap->m_VertexList[pNode->m_CornerList[1]].pos;
	TVector3 vLB = m_pMap->m_VertexList[pNode->m_CornerList[2]].pos;
	TVector3 vRB = m_pMap->m_VertexList[pNode->m_CornerList[3]].pos;
	pNode->SetRect(vLT.x, vLT.z, vRT.x - vLT.x, vLT.z - vLB.z);

	KNode::g_iNewCounter++;
	return pNode;
}
KQuadTree::KQuadTree()
{
	m_pRootNode = nullptr;
}

KQuadTree::~KQuadTree()
{
}

bool KQuadTree::ComputeStaticLodIndex(int iMaxCells)
{
	m_LodPatchlist.reserve(m_iNumPatch);
	return true;
}

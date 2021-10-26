#include "KMesh.h"
FbxColor KMesh::ReadColor(const FbxMesh* mesh,
	DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
	DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	FbxVector4 ret(0, 0, 0);
	if (dwVertexColorCount < 1)
	{
		return Value;
	}
	int dwVertexColorCountLayer = mesh->GetElementNormalCount();
	const FbxGeometryElementVertexColor* vertexColor = mesh->GetElementVertexColor(0);

	if (dwVertexColorCount > 0 && VertexColorSets != NULL)
	{
		// Crack apart the FBX dereferencing system for Color coordinates		
		switch (VertexColorSets->GetMappingMode())
		{
		case FbxLayerElement::eByControlPoint:
			switch (VertexColorSets->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				Value = VertexColorSets->GetDirectArray().GetAt(dwDCCIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = VertexColorSets->GetIndexArray().GetAt(dwDCCIndex);
				Value = VertexColorSets->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
		case FbxLayerElement::eByPolygonVertex:
			switch (VertexColorSets->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				int iColorIndex = dwVertexIndex;
				Value = VertexColorSets->GetDirectArray().GetAt(iColorIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = VertexColorSets->GetIndexArray().GetAt(dwVertexIndex);
				Value = VertexColorSets->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
			break;
		}
	}
	return Value;
}
FbxVector4 KMesh::ReadNormal(const FbxMesh* mesh,
	DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
	int controlPointIndex, int iVertexIndex)
{
	FbxVector4 result(0, 0, 0);
	if (dwVertexNormalCount < 1)
	{
		return result;
	}
	int iVertexNormalLayer = mesh->GetElementNormalCount();

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// 노말 획득 

	// 노말 벡터를 저장할 벡터 
	switch (VertexNormalSets->GetMappingMode()) 	// 매핑 모드 
	{
		// 제어점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (VertexNormalSets->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = VertexNormalSets->GetIndexArray().GetAt(controlPointIndex);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// 정점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = VertexNormalSets->GetIndexArray().GetAt(iVertexIndex);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}
FbxVector2 KMesh::ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
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

HRESULT KMesh::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	hr = KModel::CreateConstantBuffer();
	if (m_VertexList.size() <= 0) return hr;

	D3D11_BUFFER_DESC pDec;
	ZeroMemory(&pDec, sizeof(D3D11_BUFFER_DESC));
	pDec.ByteWidth = sizeof(KAnimMatrix);
	pDec.Usage = D3D11_USAGE_DEFAULT;
	pDec.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA pInitData;
	ZeroMemory(&pInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	pInitData.pSysMem = &m_matAnimMatrix;

	hr = g_pd3dDevice->CreateBuffer(&pDec, &pInitData, &m_pAnimCB);
	HRFAILED

	return hr;
}

HRESULT KMesh::LoadShaderAndInputLayout(LPCWSTR vsFile, LPCWSTR psFile)
{
	HRESULT hr;

	hr = LoadShader(vsFile, psFile);
	HRFAILED


		D3D11_INPUT_ELEMENT_DESC pInputLayout[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},

		{"INDEX", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		
	};
	UINT numLayout = sizeof(pInputLayout) / sizeof(pInputLayout[0]);



	hr = g_pd3dDevice->CreateInputLayout(pInputLayout, numLayout, m_pVStemp->GetBufferPointer(), m_pVStemp->GetBufferSize(), &m_pVertexLayout);
	HRFAILED
	m_pVStemp->Release();

	return hr;
}

HRESULT KMesh::CreateVertexBuffer()
{
	HRESULT hr;
	hr = KModel::CreateVertexBuffer();

	D3D11_BUFFER_DESC pDec;
	ZeroMemory(&pDec, sizeof(D3D11_BUFFER_DESC));
	pDec.ByteWidth = (UINT)(sizeof(PNCTIW_VERTEX) * m_WeightList.size());
	pDec.Usage = D3D11_USAGE_DEFAULT;
	pDec.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA pInitData;
	ZeroMemory(&pInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	pInitData.pSysMem = &m_WeightList.at(0);

	hr = g_pd3dDevice->CreateBuffer(&pDec, &pInitData, &m_pIWVertrexBuffer);
	HRFAILED
	return hr;
}

bool KMesh::PreRender()
{
	if (m_VertexList.size() <= 0) return true;
	// 상수 버퍼 두개 업데이트
	m_pContext->UpdateSubresource(
		m_pConstantBuffer, 0, NULL, &m_kbData, 0, 0);
	m_pContext->UpdateSubresource(
		m_pAnimCB, 0, NULL, &m_matAnimMatrix, 0, 0);
	m_pContext->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_pContext->VSSetConstantBuffers(
		1, 1, &m_pAnimCB);


	m_pContext->VSSetShader(m_pVS, NULL, 0);
	m_pContext->PSSetShader(m_pPS, NULL, 0);
	m_pContext->IASetInputLayout(m_pVertexLayout);

	ID3D11Buffer* Bf[2] = { m_pVertexBuffer, m_pIWVertrexBuffer };
	UINT pStrides[2] = {sizeof(PNCT_VERTEX), sizeof(PNCTIW_VERTEX)};
	UINT pOffsets[2] = {0,0};
	m_pContext->IASetVertexBuffers(0, 2, Bf, pStrides, pOffsets);

	m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	return true;
}

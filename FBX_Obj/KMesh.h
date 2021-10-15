#pragma once
#include <KModel.h>

class KMesh : public KModel
{
	int m_iMtrlRef;

public:
	bool PostRender(UINT iNsumIndex) override
	{
		m_pContext->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pContext->Draw(m_VertexList.size(), 0);
		return true;
	}
	bool Create(ID3D11DeviceContext* pContext, LPCWSTR vsFile, LPCWSTR psFile) override
	{
		m_pContext = pContext;
		if (CreateVertexData())
		{
			CreateConstantBuffer();
			CreateVertexBuffer();
			//CreateIndexBuffer();
			LoadShaderAndInputLayout(vsFile, psFile);
			return true;
		}
		return false;
	}
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



	
};


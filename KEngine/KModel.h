#pragma once
#include "KStd.h"
#include "KDevice.h"
struct SimpleVertex
{
	KVector3 pos;
	KVector4 color;
	SimpleVertex(KVector3 p, KVector4 c)
	{
		pos = p;
		color = c;
	}
	SimpleVertex()
	{
		pos = { 0.5f,0.5f,0.5f };
		color = { 0.5f,0.5f,0.5f,0.5f };
	}
	float x;
	float y;
	float z;
};

struct KB_Data
{
	TMatrix matWorld;
	TMatrix matView;
	TMatrix matProj;
};

class KModel
{
public:
	KB_Data m_kbData;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pConstantBuffer;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;

	ID3DBlob* m_pVStemp;
	ID3DBlob* m_pPStemp;

	std::vector<SimpleVertex> m_VertexList;
	std::vector<DWORD> m_IndexList;
	ID3D11DeviceContext* m_pContext;
	
public:
	void		SetMatrix(TMatrix* pMatWorld,
		TMatrix* pMatView, TMatrix* pMatProj);
	virtual bool	CreateVertexData();
	virtual	bool CreateIndexData() ;
	virtual	HRESULT CreateIndexBuffer() ;

public:
	HRESULT	CreateVertexBuffer() ;
	HRESULT CreateConstantBuffer();
	HRESULT	LoadShaderAndInputLayout();
	HRESULT LoadShader();
	bool LoadObject(std::wstring filename);

public:

	bool   Init(ID3D11DeviceContext* pContext);
	bool   Frame();
	bool   PreRender();
	virtual	bool   Render();
	bool	PostRender(UINT iNumIndex);
	bool   Release();
	KModel();
	virtual ~KModel() {};
};


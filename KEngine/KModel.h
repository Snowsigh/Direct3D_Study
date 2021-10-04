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
	KMatrix matWorld;
	KMatrix matView;
	KMatrix matProj;
};

class KModel
{
public:
	KB_Data m_kbData;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;

	ID3DBlob* m_pVStemp;
	ID3DBlob* m_pPStemp;

	std::vector<SimpleVertex> m_VertexList;

	ID3D11DeviceContext* m_pContext;
	
public:
	void		SetMatrix(KMatrix* pMatWorld,
		KMatrix* pMatView, KMatrix* pMatProj);
	virtual bool	CreateVertexData();

public:
	HRESULT	CreateVertexBuffer() ;
	HRESULT CreateConstantBuffer();
	HRESULT	LoadShaderAndInputLayout();
	HRESULT LoadShader();
	bool LoadObject(std::wstring filename);

public:

	bool   Init(ID3D11DeviceContext* pContext);
	bool   Frame();
virtual	bool   Render();
	bool   Release();
	KModel();
	virtual ~KModel() {};
};

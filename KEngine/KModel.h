#pragma once
#include "KStd.h"
#include "KDevice.h"


struct PNCT_VERTEX
{
	TVector3 pos;
	TVector3 normal;
	TVector4 color;
	TVector2 tex;
	PNCT_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};


struct KB_Data
{
	TMatrix matWorld;
	TMatrix matView;
	TMatrix matProj;
	TVector4 matTime;
};

class KModel
{
public:
	KB_Data m_kbData;
	UINT m_iNumIndex;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pConstantBuffer;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;

	ID3DBlob* m_pVStemp;
	ID3DBlob* m_pPStemp;

	std::vector<PNCT_VERTEX> m_VertexList;
	std::vector<DWORD> m_IndexList;
	ID3D11DeviceContext* m_pContext;
	
public:
	void		SetMatrix(TMatrix* pMatWorld,
		TMatrix* pMatView, TMatrix* pMatProj);
	virtual bool	CreateVertexData();
	virtual	bool CreateIndexData() ;
	virtual	HRESULT CreateIndexBuffer() ;

public:
	virtual	HRESULT	CreateVertexBuffer() ;
	virtual	HRESULT CreateConstantBuffer();
	virtual	HRESULT	LoadShaderAndInputLayout(LPCWSTR vsFile, LPCWSTR psFile);
	virtual	HRESULT LoadShader(LPCWSTR vsFile, LPCWSTR psFile);

	virtual	bool Create(ID3D11DeviceContext* pContext, LPCWSTR vsFile, LPCWSTR psFile);

	virtual	bool LoadObject(std::wstring filename);

public:

	virtual bool   Init();
	virtual bool   Frame();
	virtual bool   PreRender();
	virtual	bool   Render();
	virtual bool	PostRender(UINT iNumIndex);
	virtual bool   Release();
	KModel();
	virtual ~KModel() {};
};


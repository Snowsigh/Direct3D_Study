#pragma once
#include "KDevice.h"
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")
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


class KVertex
{

public:
	
	//--------------------------------------------------------------------------------------
	// ���ؽ� ���� �� ���̾ƿ�
	//--------------------------------------------------------------------------------------
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	//--------------------------------------------------------------------------------------
	// ���ؽ� �� �ȼ� ���̴�
	//--------------------------------------------------------------------------------------
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;

	ID3DBlob* m_pVStemp;
	ID3DBlob* m_pPStemp;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	std::vector<SimpleVertex> m_VertexList;

	
	ID3D11Buffer* m_pConstantBuffer;
	float m_fSpeed;
	KVector3 m_vCameraPos;
	KVector3 m_vCameraTarget;
	KB_Data m_kbData;


public:
	//--------------------------------------------------------------------------------------
	// ���� ���� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateVertexBuffer();
	//--------------------------------------------------------------------------------------
	// ���� �� �Ƚ� ���̵� �ε� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		LoadShaderAndInputLayout();
	// ���̴� ������ �� ���� 
	HRESULT LoadShader();
	HRESULT CreateConstantBuffer();
	bool LoadObject(std::wstring filename);



public:
	bool	Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool	Frame();
	bool	Render();
	bool	Release();


public:
	KVertex();
	virtual ~KVertex() {};
};



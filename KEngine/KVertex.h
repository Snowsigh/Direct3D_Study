#pragma once
#include "KDevice.h"
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")
struct SimpleVertex
{
	TVector3 pos;
	TVector4 color;
	SimpleVertex(TVector3 p, TVector4 c)
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

	bool LoadObject(std::wstring filename);



public:
	bool	Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool	Frame();
	bool	Render();
	bool	Release();


public:
	KVertex() : m_pVertexBuffer(nullptr), m_pVertexLayout(nullptr), m_pVS(nullptr), m_pPS(nullptr) {};
	virtual ~KVertex() {};
};



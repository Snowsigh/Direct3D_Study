#pragma once
#include <KCore.h>
#include <fbxsdk.h>
#include "KViewRT.h"

class Sample : public KCore
{
public:
	TVector3 m_vMoePos;

	KMiniMap m_kMini;
	KPlane m_kPlane;
	KViewRT m_kRt;
	KFbxObj m_KObj;

	TMatrix		m_matShadow;
	ID3D11PixelShader* m_pPSShadow = nullptr;

public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	
};


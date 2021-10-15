#pragma once
#include "KStd.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

class KTexture
{
public:
	ID3D11Resource* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11SamplerState* m_pSampler;
	
public:
	bool   Init();
	bool   Frame();
	bool   Render();
	bool   Release();
	bool   LoadTexture(std::wstring texFileName);
};


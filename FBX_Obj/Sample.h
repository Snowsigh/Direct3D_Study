#pragma once
#include <KCore.h>
#include "KFbxObj.h"

class Sample : public KCore
{
public:
	KMap m_kMap;
	KQuadTree m_kQuadtree;

	KFbxObj m_kFbxObj;
	KTexture m_kTex;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	
};


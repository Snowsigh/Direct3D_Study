#pragma once
#include <KCore.h>
class Sample : public KCore
{
public:
	KMap m_kMap;
	KQuadTree m_kQuadtree;

public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	
};


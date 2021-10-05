#pragma once
#include <KCore.h>
class Sample : public KCore
{
public:
	KCamera m_kCamera;
	KMap m_map;

public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};


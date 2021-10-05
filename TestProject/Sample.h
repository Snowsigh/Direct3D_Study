#pragma once
#include <KCore.h>
class Sample : public KCore
{
	KCamera m_kCamera;
	KBoxObject m_box;
	KMap m_map;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};


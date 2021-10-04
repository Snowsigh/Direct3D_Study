#pragma once
#include <KCore.h>
#include <KObject.h>
#include "KCamara.h"
class Sample : public KCore
{
public:
	KDebugCamera m_kCamera;

	KBoxObject m_box;

	
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	Sample();
};


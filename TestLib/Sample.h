#pragma once
#include <KCore.h>
#include "KObject.h"
#include "KMap.h"
class Sample : public KCore
{
public:
	float m_fSpeed;
	KVector3 m_vCameraPos;
	KVector3 m_vCameraTarget;
	KB_Data m_kbData;
public:
	KBoxObject m_boxObj[2];
	KMap m_map;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	Sample();
};


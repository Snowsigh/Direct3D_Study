#pragma once
#include <KCore.h>
#include <fbxsdk.h>

class Sample : public KCore
{
public:
	KFbxObj m_kobj;

public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	
};


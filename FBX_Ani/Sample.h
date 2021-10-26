#pragma once
#include <KCore.h>
#include <fbxsdk.h>

class Sample : public KCore
{
public:
	KFbxObj m_obj;
	std::vector<KFbxObj*> m_pObjectList;

public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	
};


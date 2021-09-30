#pragma once
#include "KDevice.h"
#include "KWindow.h"
#include "KTimer.h"
#include "KInput.h"
#include "KWrite.h"
#include "KVertex.h"
#include "KStd.h"

class KCore : public KWindow
{

private:
	bool	GameInit();
	bool	GameRun();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease();
public:

	KTimer m_kTimer;
	KWrite m_kWrite;
	KVertex m_kVertex;
	
	bool		m_bDebugText = false;
	//KInPut m_kInput;
public:
	bool Run();
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual bool	Release();
public:
}; 

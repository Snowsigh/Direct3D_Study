#pragma once
#include "KDevice.h"
#include "KWindow.h"
#include "KTimer.h"
#include "KInput.h"
#include "KStd.h"

class KCore : public KWindow
{

private:
	bool	GameInit()	override;
	bool	GameRun()	override;
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease()override;
public:
	KDevice m_kDevice;
	KTimer m_kTimer;
	//KInPut m_kInput;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual bool	Release();
public:
}; 

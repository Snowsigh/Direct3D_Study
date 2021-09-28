#pragma once
#include "KStd.h"
enum  KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};
class KInPut : public TSingleton<KInPut>
{
	friend class TSingleton<KInPut>;
	//public:
	//	static TInput* Get()
	//	{
	//		static TInput gInput;
	//		return &gInput;
	//	}
private:
	DWORD	m_dwKeyState[256];
	POINT   m_ptPos;
public:
	DWORD   GetKey(DWORD dwKey);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	KInPut();
public:
	~KInPut();
};

#define g_Input KInPut::Get()
#pragma once
#include <KCore.h>
class Sample : public KCore
{
public:
	KDebugCamera m_kCamera;
	KMap m_kMap;

	ID3D11RasterizerState* m_pRsWire;
	ID3D11RasterizerState* m_pRsSolid;
	KQuadTree m_kQuadtree;

	bool m_bWiremod = false;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


#pragma once
#include "KNode.h"
#include <KCamara.h>
#include <queue>
class KQuadTree
{
public: //LOD
	UINT m_iMaxDepth;
	UINT m_iNumcell;
	UINT m_iNumPatch;
	std::vector<DWORD> m_Indexlist;
	ID3D11Buffer* m_pIndexBuffer;
	std::vector<KLodPatch> m_LodPatchlist;
	
	KCamera* m_pCamera;
public:
	KNode* m_pRootNode;
	int			m_iNumCol;
	int			m_iNumRow;
	std::queue<KNode*>  m_Queue;
	std::vector<KNode*>  m_pLeafList;
	KMap* m_pMap;
public:
	bool	UpdateIndexList(KNode* pNode);
	HRESULT CreateIndexBuffer(KNode* pNode);
	bool UpdateVertexList(KNode* pNode);
	HRESULT CreateVertexBuffer(KNode* pNode);
	bool GetCamera(KCamera* pCamera);
public:
	void    Build(KMap* pMap);
	bool    Init();
	KNode* CreateNode(KNode* pParent, float x, float y, float w, float h);
	void	Buildtree(KNode*);
	bool    AddObject(TVector2 pos);
	KNode* FindNode(KNode* pNode, TVector2 pos);

	KNode* FindPlayerNode(TVector2 pos);
	bool	SubDivide(KNode* pNode);

	void	SetNeighborNode(KNode* pNode);
	bool LoadObject(std::wstring filename);
public:
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext);
	bool    Release();
	bool    ComputeStaticLodIndex(int iMaxCells);
	HRESULT CreateIndexBuffer(KLodPatch& patch, int iCode);
	template <typename OutputIterator>
	void	Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first);
public:
	KQuadTree();
	virtual ~KQuadTree();
};


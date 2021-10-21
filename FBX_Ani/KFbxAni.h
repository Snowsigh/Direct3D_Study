#pragma once
#include <KFbxObj.h>
class KFbxAni
{
private:
	FbxScene* m_pFbxS;
	
public:
	
	bool	m_bAnimPlay = false;
	float   m_fElpaseTime = 0.0f;
	int		m_iAnimIndex = 0;
	float m_fStartTime;
	float m_fEndTime;
	float m_fSampleTime;
public:
	bool	InitFbx(FbxScene*);
	void	ParseAnimationNode(FbxNode* pNode, KMesh* pMesh);
	void	ParseAnimation();
	void	ParseAnimStack(FbxString* szData);

};


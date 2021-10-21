#include "KFbxAni.h"

bool KFbxAni::InitFbx(FbxScene* obj)
{
	m_pFbxS = obj;
	return true;
}

void KFbxAni::ParseAnimationNode(FbxNode* pNode, KMesh* pMesh)
{
	// 에니메이션 데이터 저장
	KFbxObj mat;
	FbxAnimEvaluator* pAnim = m_pFbxS-> GetAnimationEvaluator();
	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pNode, time);
		TMatrix matGlobaDX = mat.DxConvertMatrix(mat.ConvertMatrix(matGlobal));
		pMesh->m_AnimationTrack.push_back(matGlobaDX);
		fCurrentTime += m_fSampleTime;
	}
}

void KFbxAni::ParseAnimation()
{
	FbxArray<FbxString*> AnimStackNameArray;
	m_pFbxS->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int iStack = 0; iStack < iAnimStackCount; iStack++)
	{
		ParseAnimStack(AnimStackNameArray.GetAt(iStack));
	}
}

void KFbxAni::ParseAnimStack(FbxString* szData)
{
	// Frame, Tick
// 1Frame = 160Tick
// frameSpeed = 1Sec Per 30Frame
// 1SecTick = 30 * 160 = 4800Tick
// 0 ~ 100Frame(16000/4800) = 3.3333Sec
	FbxTakeInfo* pTakeInfo = m_pFbxS->GetTakeInfo(*szData);
	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0,
		m_pFbxS->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fFrameStep = 1;
	m_fSampleTime = fFrameTime * fFrameStep;

	if (pTakeInfo)
	{
		m_fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		m_pFbxS->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);
		m_fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
}

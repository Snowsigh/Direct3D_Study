#include "KFbxAni.h"

bool KFbxAni::SetAni(FbxScene* obj, std::vector<FbxNode*> pNodeList)
{
	m_pFbxS = obj;
	m_pFNodeList = pNodeList;
	ParseAnimation();
	return true;
}
int  KFbxAni::GetFindInedx(FbxNode* pNode, std::vector<FbxNode*> pNodeList)
{
	for (int iNode = 0; iNode < pNodeList.size(); iNode++)
	{
		if (pNodeList[iNode] == pNode)
		{
			return iNode;
		}
	}
	return -1;
}

void KFbxAni::ParseAnimationNode(std::vector<KMesh*> pMeshList)
{
	// 에니메이션 데이터 저장
	FbxAnimEvaluator* pAnim = m_pFbxS->GetAnimationEvaluator();

	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		for (int iMesh = 0; iMesh < pMeshList.size(); iMesh++)
		{
			KMesh* pMesh = pMeshList[iMesh];
			FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
			TMatrix matGlobaDX = KBASE::DxConvertMatrix(KBASE::ConvertMatrixA(matGlobal));
			pMesh->m_AnimationTrack.push_back(matGlobaDX);
		}
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
	float fFrameTime = (float)FrameTime.GetSecondDouble();
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

bool KFbxAni::ParseMeshSkinning(FbxMesh* pFbxMesh, KMesh* pMesh, KSkinData* pSkindata)
{
	int iNumDeformer = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iNumDeformer == 0)
	{
		return false;
	}
	int iNumVertexCount = pFbxMesh->GetControlPointsCount();
	pSkindata->m_VertexList.resize(iNumVertexCount);

	for (int iDeformer = 0; iDeformer < iNumDeformer; iDeformer++)
	{
		FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
		int iNumCluster = pSkin->GetClusterCount();
		// 영향을 미치는 행렬이 iNumCluster 있다.
		for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);

			FbxAMatrix matXBindPose;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			FbxAMatrix matInitPostion;
			pCluster->GetTransformMatrix(matInitPostion);
			FbxAMatrix matBoneBindPos = matInitPostion.Inverse() *
				matXBindPose;
			TMatrix matBinePos =
				KBASE::DxConvertMatrix(KBASE::ConvertMatrixA(matBoneBindPos));
			// 영향을 미치는 행렬이 iClusterSize 정점에 영향을 미친다.
			int iNumVertex = pCluster->GetControlPointIndicesCount();

			FbxNode* pLinkNode = pCluster->GetLink();

			pSkindata->m_MatrixList.push_back(pLinkNode);

			int iBone = GetFindInedx(pLinkNode, m_pFNodeList);

			_ASSERT(iBone >= 0);

			pMesh->m_iBoneList.push_back(iBone);

			D3DXMatrixInverse(&matBinePos, NULL, &matBinePos);

			m_matBindPoseList[iBone] = matBinePos;

			int iMatrixIndex = pSkindata->m_MatrixList.size() - 1;
			//ControlPoint(제어점) 정점리스트
			int* iIndex = pCluster->GetControlPointIndices();
			// 가중치리스트
			double* pWeight = pCluster->GetControlPointWeights();
			for (int i = 0; i < iNumVertex; i++)
			{
				pSkindata->m_VertexList[iIndex[i]].m_IndexList.push_back(iMatrixIndex);
				pSkindata->m_VertexList[iIndex[i]].m_WegihtList.push_back(pWeight[i]);
				//iIndex[i] 정점은  iMatrixIndex행렬이 pWeight[i]=1 가중치로 영향을 미친다.				
			}
		}
	}
	return true;
}

#include "KFbxAni.h"

bool KFbxAni::SetAni(FbxScene* obj)
{
	m_pFbxS = obj;
	ParseAnimation();
	return true;
}
KMesh*  KFbxAni::GetFindInedx(FbxNode* pNode, std::vector<KMesh*> pMeshList)
{
	for (int iNode = 0; iNode < pMeshList.size(); iNode++)
	{
		KMesh* pMesh = pMeshList[iNode];
		if (pMesh->m_pFbxNode == pNode)
		{
			return pMesh;
		}
	}
	return nullptr;
}

void KFbxAni::ParseAnimationNode(std::vector<KMesh*> pMeshList)
{
	// ���ϸ��̼� ������ ����
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
//

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
	m_pFbxS->GetAnimationEvaluator()->Reset();
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
		// ������ ��ġ�� ����� iNumCluster �ִ�.
		pMesh->m_matBindPoseList.resize(iNumCluster);
		for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);
			// ������ ��ġ�� ����� iClusterSize ������ ������ ��ģ��.
			int iNumVertex = pCluster->GetControlPointIndicesCount();

			FbxAMatrix matXBindPose, matInitPostion;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matInitPostion);
			FbxAMatrix matBoneBindPos = matInitPostion.Inverse() * matXBindPose;
			TMatrix matBinePos = KBASE::DxConvertMatrix(KBASE::ConvertMatrixA(matBoneBindPos));
			D3DXMatrixInverse(&matBinePos, NULL, &matBinePos);
			pMesh->m_matBindPoseList[iCluster] = matBinePos;

			FbxNode* pLinkNode = pCluster->GetLink();
			pMesh->m_pFNodeList.push_back(pLinkNode);
			//ControlPoint(������) ��������Ʈ
			int* iIndex = pCluster->GetControlPointIndices();
			// ����ġ����Ʈ
			double* pWeight = pCluster->GetControlPointWeights();
			for (int i = 0; i < iNumVertex; i++)
			{
				pSkindata->m_VertexList[iIndex[i]].m_IndexList.push_back(iCluster);
				pSkindata->m_VertexList[iIndex[i]].m_WegihtList.push_back(pWeight[i]);
				//iIndex[i] ������  iMatrixIndex����� pWeight[i]=1 ����ġ�� ������ ��ģ��.				
			}
		}
	}
	return true;
}
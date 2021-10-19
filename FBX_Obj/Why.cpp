void KFbxObj::PreProcess(FbxNode* pNode)
{
	if (pNode->GetCamera() || pNode->GetLight())
	{
		return;
	}

	int iNumFbxMtrl = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	if (GetRootMtrl(pFbxMaterial) == -1)
	{
		if (iNumFbxMtrl > 1) //서브존재
		{
			KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMtrl; iSub++)
			{
				FbxSurfaceMaterial* pFbxSubMtrl = pNode->GetMaterial(iSub);
				_ASSERT(pFbxSubMtrl != nullptr);
				KMtrl* pSubMtrl = new KMtrl(pNode, pFbxMaterial);
				pMtrl->m_pSubMtrl.push_back(pSubMtrl);
				pFbxMaterial = nullptr;
			}
			m_pMtrlList.push_back(pMtrl);

		}
		else // 서브없음
		{
			if (pFbxMaterial != nullptr)
			{
				KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
				m_pMtrlList.push_back(pMtrl);
			}
		}
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		FbxNodeAttribute::EType type =
			pChildNode->GetNodeAttribute()->GetAttributeType();
		if (ChildNode->GetNodeAttribute() != nullptr)
		{
			m_pFbxNodeList.push_back(pChildNode);
		}
		PreProcess(pChildNode);
	}

}

if (pNode->GetCamera() || pNode->GetLight())
	{
		return;
	}
	int iNumFbxMaterial = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	if (GetRootMtrl(pFbxMaterial) == -1)
	{
		if (iNumFbxMaterial > 1 )
		{
			TMtrl* pMtrl = new TMtrl(pNode, pFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				FbxSurfaceMaterial* pFbxSubMaterial = pNode->GetMaterial(iSub);
				_ASSERT(pFbxSubMaterial != nullptr);
				TMtrl* pSubMtrl = new TMtrl(pNode, pFbxSubMaterial);
				pMtrl->m_pSubMtrl.push_back(pSubMtrl);
			}
			m_pFbxMaterialList.push_back(pMtrl);
		}
		else
		{
			if (pFbxMaterial != nullptr)
			{
				TMtrl* pMtrl = new TMtrl(pNode, pFbxMaterial);
				m_pFbxMaterialList.push_back(pMtrl);
			}
		}
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);	
		FbxNodeAttribute::EType type = 
			pChildNode->GetNodeAttribute()->GetAttributeType();
		if (/*type == FbxNodeAttribute::eMesh ||
			type == FbxNodeAttribute::eSkeleton*/
			pChildNode->GetNodeAttribute() != nullptr)
		{
			m_pFbxNodeList.push_back(pChildNode);
		}
		PreProcess(pChildNode);
	}
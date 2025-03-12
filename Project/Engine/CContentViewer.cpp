#include "pch.h"
#include "CContentViewer.h"

#include "CAssetMgr.h"

#include "CTreeUI.h"

CContentViewer::CContentViewer(wstring _Name)
	: CImguiObject(_Name)
{
	m_AssetTree = new CTreeUI(L"AssetTree");
}

CContentViewer::~CContentViewer()
{
	delete m_AssetTree;
}

void CContentViewer::Update()
{
	if (CAssetMgr::GetInst()->GetRenew())
		Renew();
}

void CContentViewer::Render()
{
	m_AssetTree->Render();
}

void CContentViewer::Renew()
{
	for (int i = 0; i < static_cast<UINT>(ASSET_TYPE::ASSET_END); ++i)
	{
		TreeNode* pNode = m_AssetTree->AddChildNode(m_AssetTree->GetRoot(), to_str(ASSET_TYPE_WSTR[i]), 0);
		for (auto& pair : CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i))
		{
			AssetPtr<CAsset> pAsset = pair.second;
			m_AssetTree->AddChildNode(pNode, to_str(pAsset.Get()->GetName()), (DWORD_PTR)pAsset.Get());
		}
	}
}

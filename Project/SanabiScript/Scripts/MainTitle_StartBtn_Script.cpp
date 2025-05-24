#include "pch.h"
#include "MainTitle_StartBtn_Script.h"

#include "Engine/CUICom.h"
#include "Engine/CGameObject.h"
#include "Engine/CMeshRender.h"
#include "Engine/CAssetMgr.h"

MainTitle_StartBtn_Script::MainTitle_StartBtn_Script()
	: m_Init(false)
{
}

MainTitle_StartBtn_Script::MainTitle_StartBtn_Script(const MainTitle_StartBtn_Script& _Origin)
	: m_Init(false)
{
}

MainTitle_StartBtn_Script::~MainTitle_StartBtn_Script()
{
}


void MainTitle_StartBtn_Script::Tick()
{
	CUICom* pUICom = GetOwner()->UICom();
	CMeshRender* pMeshRender = GetOwner()->MeshRender();
	if (pUICom == nullptr || pMeshRender == nullptr || !IsRenderable(GetOwner()))
		return;

	if (!m_Init)
	{
		m_Init = false;
		pUICom->SetClickEvent(&ClickEvent);
	}

	if (pUICom->GetClicked())
	{
		pMeshRender->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture2D>(L"Texture2D\\UI_MainScene_SelectBox_Selected.png"));
	}
	else
	{
		pMeshRender->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture2D>(L"Texture2D\\UI_MainScene_SelectBox.png"));
	}
}

int MainTitle_StartBtn_Script::Save(fstream& _Stream)
{
	return S_OK;
}

int MainTitle_StartBtn_Script::Load(fstream& _Stream)
{
	return S_OK;
}

void MainTitle_StartBtn_Script::ClickEvent(CGameObject* _Owner)
{
	int a = 0;
}

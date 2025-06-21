#include "pch.h"
#include "MainTitle_StartBtn_Script.h"

#include "Engine/CUICom.h"
#include "Engine/CGameObject.h"
#include "Engine/CMeshRender.h"
#include "Engine/CFSM.h"

#include "Engine/CAssetMgr.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CTaskMgr.h"
#include "Engine/CTimeMgr.h"

MainTitle_StartBtn_Script::MainTitle_StartBtn_Script()
	: CScript()
	, m_Init(false)
	, m_Timer(0)
{
}

MainTitle_StartBtn_Script::MainTitle_StartBtn_Script(const MainTitle_StartBtn_Script& _Origin)
	: CScript(_Origin)
	, m_Init(false)
	, m_Timer(0)
{
}

MainTitle_StartBtn_Script::~MainTitle_StartBtn_Script()
{
}


void MainTitle_StartBtn_Script::Tick()
{
	CUICom* pUICom = GetOwner()->UICom();
	if (pUICom == nullptr || !IsRenderable(GetOwner()))
		return;

	if (!m_Init)
	{
		m_Init = false;
		pUICom->SetClickEvent(&ClickEvent);
		pUICom->SetActive(false);
	}

	// 백그라운드 객체가 루프일 때 타이머로 알파값 조절 및 활성화
	if (CGameObject* pBG = CLevelMgr::GetInst()->FindObject(LAYER::Background, L"MainTitle_Bg"); pBG)
	{
		if (CFSM_State* pState = pBG->FSM()->GetCurrentState(); pState)
		{
			if (pState->GetName() == L"class MainTitle_Bg_State_Loop")
			{
				if (m_Timer < 1)
					m_Timer += DT;
				else
				{
					m_Timer = 1.f;
				}
			}
		}
	}

	pUICom->SetAlpha(m_Timer);
	if (m_Timer == 1.f && !pUICom->GetActive())
		pUICom->SetActive(true);


	// 클릭된 상태일 때 텍스쳐 변경
	if (pUICom->GetClicked())
	{
		pUICom->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture2D>(L"Texture2D\\UI_MainScene_SelectBox_Selected.png"));
	}
	else
	{
		pUICom->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture2D>(L"Texture2D\\UI_MainScene_SelectBox.png"));
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
	CLevel* pLv = CLevelMgr::GetInst()->FindLevel(L"Level\\Stage1_1.level");
	if (pLv)
		CTaskMgr::GetInst()->AddTask(TASK_TYPE::CHANGE_LEVEL, (DWORD_PTR)pLv, (DWORD_PTR)true);
}

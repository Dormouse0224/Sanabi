#include "pch.h"
#include "MainTitleBgScript.h"

#include "Engine/CAssetMgr.h"

#include "Engine/CTimeMgr.h"
#include "Engine/CGameObject.h"
#include "Engine/CUICom.h"
#include "Engine/CRenderComponent.h"
#include "Engine/CFSM.h"
#include "Engine/CFSM_State.h"
#include "Engine/CLevelMgr.h"

MainTitleBgScript::MainTitleBgScript()
	: CScript()
	, m_Timer(0.f)
{
}

MainTitleBgScript::MainTitleBgScript(const MainTitleBgScript& _Origin)
	: CScript(_Origin)
	, m_Timer(0.f)
{
}

MainTitleBgScript::~MainTitleBgScript()
{
}

void MainTitleBgScript::Tick()
{

	if (CGameObject* pObj = CLevelMgr::GetInst()->FindObject(LAYER::Background, L"MainTitle_Bg"); pObj)
	{
		if (CFSM_State* pState = pObj->FSM()->GetCurrentState(); pState)
		{
			if (pState->GetName() == L"class MainTitle_Bg_State_Loop")
			{
				if (m_Timer < 2)
					m_Timer += DT;
				else
					m_Timer = 2.f;
			}
		}
	}
	GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(FLOAT_0, m_Timer);
}

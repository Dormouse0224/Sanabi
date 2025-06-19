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
			// 만약 현재 상태가 루프 상태인 경우 타이머값을 0에서 1까지 단계적으로 증가시킴
			if (pState->GetName() == L"class MainTitle_Bg_State_Loop")
			{
				if (m_Timer < 1)
					m_Timer += DT;
				else
					m_Timer = 1.f;
			}
		}
	}

	// 머티리얼의 알파 틴트값을 타이머값으로 설정 (0 ~ 1)
	if (GetOwner()->GetRenderComponent()->GetUsingDynamic())
	{
		GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(FLOAT_0, m_Timer);
	}
}

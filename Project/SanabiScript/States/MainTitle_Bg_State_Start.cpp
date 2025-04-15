#include "pch.h"
#include "MainTitle_Bg_State_Start.h"

#include "Engine/CAssetMgr.h"
#include "Engine/CTimeMgr.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CTransform.h"
#include "Engine/CEngine.h"
#include "Engine/CSound.h"
#include "Engine/CFlipbookRender.h"

MainTitle_Bg_State_Start::MainTitle_Bg_State_Start()
	: CFSM_State()
{
	SetConstVec<float>(vector<float>{30.f});
}

MainTitle_Bg_State_Start::~MainTitle_Bg_State_Start()
{
}

void MainTitle_Bg_State_Start::Tick()
{
	float timer = 1.f;
	if (GetConst<float>(0, &timer) && timer > 0)
	{
		m_Owner->GetOwner()->Transform()->SetRelativePos(-CEngine::GetInst()->GetResolution().x * (timer / 30)
			, CEngine::GetInst()->GetResolution().y * (timer / 30), 100);
		SetConst<float>(0, timer - DT);
	}
}

void MainTitle_Bg_State_Start::Begin()
{
	m_Owner->GetOwner()->Transform()->SetRelativePos(-CEngine::GetInst()->GetResolution().x, -CEngine::GetInst()->GetResolution().y, 100);
	AssetPtr<CSound> pBGM = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\BGM_Event_MainTheme.wav");
	if (pBGM.Get())
		pBGM->Play(0, 0.05, true);
	if (auto render = m_Owner->GetOwner()->FlipbookRender(); render)
		render->Play(L"Flipbook\\MainTitle_Bg_start.flip", 10, true);
}

void MainTitle_Bg_State_Start::End()
{
	m_Owner->GetOwner()->Transform()->SetRelativePos(0, 0, 100);
}

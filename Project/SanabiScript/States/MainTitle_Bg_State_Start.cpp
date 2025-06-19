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
	// 초기 타이머를 30초로 저장
	SetConstVec<float>(vector<float>{30.f});
}

MainTitle_Bg_State_Start::~MainTitle_Bg_State_Start()
{
}

void MainTitle_Bg_State_Start::Tick()
{
	float timer = 1.f;
	// 저장된 타이머 값 가져와서 확인, 타이머가 0 이상인 경우 해당 비율만큼 배경을 좌상단에서 중앙쪽으로 이동
	if (GetConst<float>(0, &timer) && timer > 0)
	{
		m_Owner->GetOwner()->Transform()->SetRelativePos(-CEngine::GetInst()->GetResolution().x * (timer / 30)
			, CEngine::GetInst()->GetResolution().y * (timer / 30), 100);

		// DT 만큼 타이머를 차감 후 저장
		SetConst<float>(0, timer - DT);
	}
}

void MainTitle_Bg_State_Start::Begin()
{
	// 배경을 죄상단으로 이동
	m_Owner->GetOwner()->Transform()->SetRelativePos(-CEngine::GetInst()->GetResolution().x, -CEngine::GetInst()->GetResolution().y, 100);

	// 배경 음악 재생
	AssetPtr<CSound> pBGM = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\BGM_Event_MainTheme.wav");
	if (pBGM.Get())
		pBGM->Play(0, 0.05, true);

	// 배경 애니메이션 재생
	if (auto render = m_Owner->GetOwner()->FlipbookRender(); render)
		render->Play(L"Flipbook\\MainTitle_Bg_start.flip", 10, true);
}

void MainTitle_Bg_State_Start::End()
{
	float timer = 1.f;
	// 저장된 타이머 값 가져와서 확인, 타이머가 0 이상인 경우 도중에 중단된 것이므로 사운드 종료
	if (GetConst<float>(0, &timer) && timer > 0)
	{
		FMOD::ChannelGroup* master = nullptr;
		CEngine::GetInst()->GetFMODSystem()->getMasterChannelGroup(&master);
		master->stop();
	}
	else
	{
		// 배경을 화면 정중앙에 고정
		m_Owner->GetOwner()->Transform()->SetRelativePos(0, 0, 100);
	}
}

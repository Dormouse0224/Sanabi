#include "pch.h"
#include "Stage1_BGM.h"

#include "Engine/CAssetMgr.h"

Stage1_BGM::Stage1_BGM()
	: CFSM_State()
{
}

Stage1_BGM::Stage1_BGM(const Stage1_BGM& _Other)
	: CFSM_State(_Other)
{
}

Stage1_BGM::~Stage1_BGM()
{
}

void Stage1_BGM::Tick()
{
	AssetPtr<CSound> pBGM0 = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\BGM_Chap1_Main_Intro.wav");
	AssetPtr<CSound> pBGM1 = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\BGM_Chap1_Main_Loop.wav");
	if (pBGM0.Get() && pBGM1.Get() && !pBGM0->IsPlaying() && !pBGM1->IsPlaying())
	{
		pBGM1->Play(0, 0.05, true);
	}
		
}

void Stage1_BGM::Begin()
{
	// 배경 음악 재생
	AssetPtr<CSound> pAMB = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\AMB_CHAP1_Raining_Loop.wav");
	if (pAMB.Get())
		pAMB->Play(0, 0.05, true);

	AssetPtr<CSound> pBGM = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\BGM_Chap1_Main_Intro.wav");
	if (pBGM.Get())
		pBGM->Play(1, 0.05, true);
}

void Stage1_BGM::End()
{
	// 배경 음악 종료
	AssetPtr<CSound> pBGM0 = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\AMB_CHAP1_Raining_Loop.wav");
	if (pBGM0.Get())
		pBGM0->Stop();
	AssetPtr<CSound> pBGM1 = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\BGM_Chap1_Main_Intro.wav");
	if (pBGM1.Get())
		pBGM1->Stop();
	AssetPtr<CSound> pBGM2 = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\BGM_Chap1_Main_Loop.wav");
	if (pBGM2.Get())
		pBGM2->Stop();
}

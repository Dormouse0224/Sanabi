#include "pch.h"
#include "MainTitle_Bg_State_Loop.h"

#include "Engine/CEngine.h"
#include "Engine/CFSM.h"
#include "Engine/CFlipbookRender.h"

MainTitle_Bg_State_Loop::MainTitle_Bg_State_Loop()
{
}

MainTitle_Bg_State_Loop::~MainTitle_Bg_State_Loop()
{
}

void MainTitle_Bg_State_Loop::Tick()
{
}

void MainTitle_Bg_State_Loop::Begin()
{
	if (auto render = m_Owner->GetOwner()->FlipbookRender(); render)
		render->Play(0, 10, true);
}

void MainTitle_Bg_State_Loop::End()
{
	FMOD::ChannelGroup* master = nullptr;
	CEngine::GetInst()->GetFMODSystem()->getMasterChannelGroup(&master);
	master->stop();
}

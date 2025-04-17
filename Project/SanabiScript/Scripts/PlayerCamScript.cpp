#include "pch.h"
#include "PlayerCamScript.h"

#include "Engine/CLevelMgr.h"

#include "Engine/CTransform.h"


PlayerCamScript::PlayerCamScript()
{
}

PlayerCamScript::PlayerCamScript(const PlayerCamScript& _Origin)
{
}

PlayerCamScript::~PlayerCamScript()
{
}

void PlayerCamScript::Tick()
{
	CGameObject* pPlayer = CLevelMgr::GetInst()->FindObject(LAYER::Player, L"Player");
	if (pPlayer)
	{
		Vec3 playerPos = pPlayer->Transform()->GetRelativePos();
		GetOwner()->Transform()->SetRelativePos(playerPos.x, playerPos.y, 0);
	}
}

void PlayerCamScript::Render_Script()
{
}

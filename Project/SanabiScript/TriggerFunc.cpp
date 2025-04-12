#include "pch.h"
#include "TriggerFunc.h"

bool MainTitle_Bg_Start_Loop(CFSM_State* _Origin, CFSM_State* _Dest)
{
	float time = 1.f;
	if (!_Origin->GetConst<float>(0, &time))
		return false;

	if (time <= 0)
		return true;
	else
		return false;
}

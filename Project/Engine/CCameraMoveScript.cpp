#include "pch.h"
#include "CCameraMoveScript.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CTransform.h"
#include "CCamera.h"

CCameraMoveScript::CCameraMoveScript()
	: m_CamSpeed(500.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::Tick()
{
	// 카메라 컴포넌트가 없으면 종료
	if (!Camera())
		return;

	PROJ_TYPE type = Camera()->GetProjType();

	if (PROJ_TYPE::PERSPECTIVE == type)
		Move_Perspective();
	else
		Move_OrthoGraphic();
}

void CCameraMoveScript::Move_Perspective()
{
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);

	if (KEY_PRESSED(Keyboard::W))
		vPos += vFront * DT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::S))
		vPos -= vFront * DT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::A))
		vPos -= vRight * DT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::D))
		vPos += vRight * DT * m_CamSpeed;

	Transform()->SetRelativePos(vPos);


	if (!KEY_PRESSED(Keyboard::MOUSE_RBTN))
		return;

	Vec2 vDragDir = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.x += vDragDir.y * DT * 1800.f;
	vRot.y += vDragDir.x * DT * 1800.f;
	Transform()->SetRelativeRotation(vRot);
}

void CCameraMoveScript::Move_OrthoGraphic()
{
	Vec3 vRot = Vec3(0.f, 0.f, 0.f);
	Transform()->SetRelativeRotation(vRot);

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(Keyboard::W))
		vPos.y += DT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::S))
		vPos.y -= DT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::A))
		vPos.x -= DT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::D))
		vPos.x += DT * m_CamSpeed;

	Transform()->SetRelativePos(vPos);
}
#include "pch.h"
#include "CCameraMoveScript.h"

#include "Engine\CTimeMgr.h"
#include "Engine\CKeyMgr.h"
#include "Engine\CLevelMgr.h"

#include "Engine\CTransform.h"
#include "Engine\CCamera.h"
#include "Engine\CLevel.h"

CCameraMoveScript::CCameraMoveScript()
	: m_CamSpeed(500.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::Tick()
{
	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() != LEVEL_STATE::PLAY)
		return;

	// 카메라 컴포넌트가 없으면 종료
	if (!Camera())
		return;

	PROJ_TYPE type = Camera()->GetProjType();

	if (PROJ_TYPE::PERSPECTIVE == type)
		Move_Perspective();
	else
		Move_OrthoGraphic();
}

int CCameraMoveScript::Load(fstream& _Stream)
{
	_Stream.read(reinterpret_cast<char*>(&m_CamSpeed), sizeof(float));

	return S_OK;
}

int CCameraMoveScript::Save(fstream& _Stream)
{
	_Stream.write(reinterpret_cast<char*>(&m_CamSpeed), sizeof(float));

	return S_OK;
}

void CCameraMoveScript::Move_Perspective()
{
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);

	if (KEY_PRESSED(Keyboard::W))
		vPos += vFront * EngineDT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::S))
		vPos -= vFront * EngineDT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::A))
		vPos -= vRight * EngineDT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::D))
		vPos += vRight * EngineDT * m_CamSpeed;

	Transform()->SetRelativePos(vPos);


	if (!KEY_PRESSED(Keyboard::MOUSE_RBTN))
		return;

	Vec2 vDragDir = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetRelativeRot();
	//Vec4 q1 = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(vDragDir.y * EngineDT * 100.f));
	//Vec4 q2 = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(vDragDir.x * EngineDT * 100.f));
	vRot.x += vDragDir.y * EngineDT * 10.f;
	vRot.y += vDragDir.x * EngineDT * 10.f;
	//vRot = XMQuaternionMultiply(vRot, XMQuaternionMultiply(q1, q2));
	Transform()->SetRelativeRot(vRot);
}

void CCameraMoveScript::Move_OrthoGraphic()
{
	Vec4 vRot = Vec4(0.f, 0.f, 0.f, 1.f);
	Transform()->SetRelativeRot(vRot);

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(Keyboard::W))
		vPos.y += EngineDT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::S))
		vPos.y -= EngineDT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::A))
		vPos.x -= EngineDT * m_CamSpeed;
	if (KEY_PRESSED(Keyboard::D))
		vPos.x += EngineDT * m_CamSpeed;

	Transform()->SetRelativePos(vPos);
}

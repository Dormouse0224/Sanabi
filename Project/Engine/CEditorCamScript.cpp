#include "pch.h"
#include "CEditorCamScript.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CLevelMgr.h"

#include "CTransform.h"
#include "CCamera.h"
#include "CLevel.h"

CEditorCamScript::CEditorCamScript()
	: m_CamSpeedLin(500.f)
	, m_CamSpeedAng(100.f)
{
}

CEditorCamScript::~CEditorCamScript()
{
}

void CEditorCamScript::Tick()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel == nullptr || pLevel->GetState() != LEVEL_STATE::STOP)
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

int CEditorCamScript::Load(fstream& _Stream)
{
	_Stream.read(reinterpret_cast<char*>(&m_CamSpeedLin), sizeof(float));

	return S_OK;
}

int CEditorCamScript::Save(fstream& _Stream)
{
	_Stream.write(reinterpret_cast<char*>(&m_CamSpeedLin), sizeof(float));

	return S_OK;
}

void CEditorCamScript::Move_Perspective()
{
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);

	if (KEY_PRESSED(Keyboard::W))
		vPos += vFront * EngineDT * m_CamSpeedLin;
	if (KEY_PRESSED(Keyboard::S))
		vPos -= vFront * EngineDT * m_CamSpeedLin;
	if (KEY_PRESSED(Keyboard::A))
		vPos -= vRight * EngineDT * m_CamSpeedLin;
	if (KEY_PRESSED(Keyboard::D))
		vPos += vRight * EngineDT * m_CamSpeedLin;

	Transform()->SetRelativePos(vPos);


	if (!KEY_PRESSED(Keyboard::MOUSE_RBTN))
		return;

	Vec2 vDragDir = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetRelativeRot();
	//Vec4 q1 = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(vDragDir.y * EngineDT * 100.f));
	//Vec4 q2 = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(vDragDir.x * EngineDT * 100.f));
	vRot.x += vDragDir.y * EngineDT * m_CamSpeedAng;
	vRot.y += vDragDir.x * EngineDT * m_CamSpeedAng;
	//vRot = XMQuaternionMultiply(vRot, XMQuaternionMultiply(q1, q2));
	Transform()->SetRelativeRot(vRot);
}

void CEditorCamScript::Move_OrthoGraphic()
{
	Vec4 vRot = Vec4(0.f, 0.f, 0.f, 1.f);
	Transform()->SetRelativeRot(vRot);

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(Keyboard::W))
		vPos.y += EngineDT * m_CamSpeedLin;
	if (KEY_PRESSED(Keyboard::S))
		vPos.y -= EngineDT * m_CamSpeedLin;
	if (KEY_PRESSED(Keyboard::A))
		vPos.x -= EngineDT * m_CamSpeedLin;
	if (KEY_PRESSED(Keyboard::D))
		vPos.x += EngineDT * m_CamSpeedLin;

	Transform()->SetRelativePos(vPos);
}

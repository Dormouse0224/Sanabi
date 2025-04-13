#include "pch.h"

#include "CRenderMgr.h"
#include "CGameObject.h"
#include "CRenderComponent.h"

void DrawDebugRect(Vec4 _Color, Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WorldRotation
	, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;
	info.Color = _Color;
	info.WorldPos = _WorldPos;
	info.WorldScale = _WorldScale;
	info.WorldRotation = _WorldRotation;
	info.MatWorld = XMMatrixIdentity();
	info.CurTime = 0.f;
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(Vec4 _Color, const Matrix& _matWorld, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;
	info.Color = _Color;
	info.MatWorld = _matWorld;
	info.CurTime = 0.f;
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec4 _Color, Vec3 _WorldPos, float _Radius, Vec3 _WorldRotation
	, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::CIRCLE;
	info.Color = _Color;
	info.WorldPos = _WorldPos;
	info.WorldScale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);
	info.WorldRotation = _WorldRotation;
	info.CurTime = 0.f;
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

bool IsRenderable(CGameObject* obj)
{
	auto* render = obj->GetRenderComponent();
	return render &&
		render->GetMesh().Get() &&
		render->GetMaterial().Get() &&
		render->GetMaterial()->GetShader().Get();
}
#include "pch.h"
//#include "CCollider2D.h"
//
//#include "CTransform.h"
//#include "CScript.h"
//
//
//CCollider2D::CCollider2D()
//	: CComponent(COMPONENT_TYPE::COLLIDER2D)
//	, m_Scale(Vec3(1.f, 1.f, 0.f))
//	, m_OverlapCount(0)
//	, m_NotifyParent(false)
//	, m_IndependentScale(false)
//{
//}
//
//CCollider2D::~CCollider2D()
//{
//}
//
//void CCollider2D::FinalTick()
//{
//	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
//	Matrix matTrans = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);
//	m_matColWorld = matScale * matTrans;
//
//	if (m_IndependentScale)
//	{
//		Vec3 Scale = Transform()->GetWorldScale();
//		Matrix matScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(Scale.x, Scale.y, Scale.z));
//		m_matColWorld = m_matColWorld * matScaleInv * Transform()->GetWorldMat();
//	}
//	else
//	{
//		m_matColWorld *= Transform()->GetWorldMat();
//	}
//
//	// DebugRender 로 충돌체의 위치를 표시함
//	// 다른 충돌체와 겹쳐있으면 Red, 아무 충돌체하고 겹쳐있지 않으면 Green
//	Vec4 vColor;
//	if (m_OverlapCount != 0)
//		vColor = Vec4(1.f, 0.f, 0.f, 1.f);
//	else
//		vColor = Vec4(0.f, 1.f, 0.f, 1.f);
//
//	DrawDebugRect(vColor, m_matColWorld, false);
//}
//
//void CCollider2D::BeginOverlap(CCollider2D* _Other)
//{
//	++m_OverlapCount;
//	NotifyBeginOverlap(_Other);
//}
//
//void CCollider2D::Overlap(CCollider2D* _Other)
//{
//	NotifyOverlap(_Other);
//}
//
//void CCollider2D::EndOverlap(CCollider2D* _Other)
//{
//	--m_OverlapCount;
//	NotifyEndOverlap(_Other);
//}
//
//void CCollider2D::NotifyBeginOverlap(CCollider2D* _Other)
//{
//	CGameObject* pObject = GetOwner();
//
//	const vector<CScript*>& vecScripts = pObject->GetScripts();
//	for (size_t i = 0; i < vecScripts.size(); ++i)
//	{
//		vecScripts[i]->BeginOverlap(this, _Other->GetOwner(), _Other);
//	}
//
//	if (m_NotifyParent)
//	{
//		pObject = pObject->GetParent();
//
//		while (pObject)
//		{
//			const vector<CScript*>& vecScripts = pObject->GetScripts();
//			for (size_t i = 0; i < vecScripts.size(); ++i)
//			{
//				vecScripts[i]->BeginOverlap(this, _Other->GetOwner(), _Other);
//			}
//			pObject = pObject->GetParent();
//		}
//	}
//}
//
//void CCollider2D::NotifyOverlap(CCollider2D* _Other)
//{
//	CGameObject* pObject = GetOwner();
//
//	const vector<CScript*>& vecScripts = pObject->GetScripts();
//	for (size_t i = 0; i < vecScripts.size(); ++i)
//	{
//		vecScripts[i]->Overlap(this, _Other->GetOwner(), _Other);
//	}
//
//	if (m_NotifyParent)
//	{
//		pObject = pObject->GetParent();
//
//		while (pObject)
//		{
//			const vector<CScript*>& vecScripts = pObject->GetScripts();
//			for (size_t i = 0; i < vecScripts.size(); ++i)
//			{
//				vecScripts[i]->Overlap(this, _Other->GetOwner(), _Other);
//			}
//			pObject = pObject->GetParent();
//		}
//	}
//}
//
//void CCollider2D::NotifyEndOverlap(CCollider2D* _Other)
//{
//	CGameObject* pObject = GetOwner();
//
//	const vector<CScript*>& vecScripts = pObject->GetScripts();
//	for (size_t i = 0; i < vecScripts.size(); ++i)
//	{
//		vecScripts[i]->EndOverlap(this, _Other->GetOwner(), _Other);
//	}
//
//	if (m_NotifyParent)
//	{
//		pObject = pObject->GetParent();
//
//		while (pObject)
//		{
//			const vector<CScript*>& vecScripts = pObject->GetScripts();
//			for (size_t i = 0; i < vecScripts.size(); ++i)
//			{
//				vecScripts[i]->EndOverlap(this, _Other->GetOwner(), _Other);
//			}
//			pObject = pObject->GetParent();
//		}
//	}
//}
#include "pch.h"
#include "CUICom.h"

#include "CKeyMgr.h"
#include "CEngine.h"

#include "CTransform.h"

CUICom::CUICom()
	:CComponent(COMPONENT_TYPE::UICOM)
	, m_MouseHover(false)
	, m_Clicked(false)
{
}

CUICom::~CUICom()
{
}

void CUICom::FinalTick()
{
	// UI 의 화면상 위치 계산 후 마우스 커서의 호버링 체크
	Vec3 wpos(Transform()->GetWorldPos());
	Vec3 scale = Transform()->GetWorldScale();
	Vec2 cursor = CKeyMgr::GetInst()->GetMousePos();

	Vec4 vpos = XMVector3Transform(wpos, g_Trans.matView);
	Vec3 vposLT(vpos.x - scale.x * 0.5f, vpos.y + scale.y * 0.5f, vpos.z);
	Vec3 vposRB(vpos.x + scale.x * 0.5f, vpos.y - scale.y * 0.5f, vpos.z);

	Vec4 pposLT = XMVector3Transform(vposLT, g_Trans.matProj);
	Vec4 pposRB = XMVector3Transform(vposRB, g_Trans.matProj);

	Vec2 sposLT((pposLT.x + 1.0f) * 0.5f * CEngine::GetInst()->GetResolution().x, (1.0f - pposLT.y) * 0.5f * CEngine::GetInst()->GetResolution().y);
	Vec2 sposRB((pposRB.x + 1.0f) * 0.5f * CEngine::GetInst()->GetResolution().x, (1.0f - pposRB.y) * 0.5f * CEngine::GetInst()->GetResolution().y);

	if (sposLT.x < cursor.x && sposLT.y < cursor.y && pposRB.x > cursor.x && pposRB.y > cursor.y)
		m_MouseHover = true;
	else
		m_MouseHover = false;

	// 마우스 좌클릭 이벤트 체크
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::TAP && m_MouseHover)
		m_Clicked = true;

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::PRESSED && m_MouseHover && m_Clicked)
	{
		if (m_KeydownEvent)
			m_KeydownEvent();
	}

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::RELEASE && m_MouseHover && m_Clicked)
	{
		m_Clicked = false;
		if (m_ClickEvent)
			m_ClickEvent();
	}

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::NONE)
		m_Clicked = false;

}

int CUICom::Save(fstream& _Stream)
{
	return 0;
}

int CUICom::Load(fstream& _Stream)
{
	return 0;
}

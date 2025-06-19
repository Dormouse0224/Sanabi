#include "pch.h"
#include "CUICom.h"

#include "CKeyMgr.h"
#include "CEngine.h"

#include "CTransform.h"
#include "CFont.h"

CUICom::CUICom()
	:CRenderComponent(COMPONENT_TYPE::UICOM)
	, m_ClickEvent(nullptr)
	, m_KeydownEvent(nullptr)
	, m_MouseHover(false)
	, m_Clicked(false)
	, m_vecUIText{}
{
}

CUICom::CUICom(const CUICom& _Other)
	:CRenderComponent(_Other)
	, m_ClickEvent(_Other.m_ClickEvent)
	, m_KeydownEvent(_Other.m_KeydownEvent)
	, m_MouseHover(false)
	, m_Clicked(false)
	, m_vecUIText{}
{
	for (int i = 0; i < _Other.m_vecUIText.size(); ++i)
	{
		tUITextDesc* pDesc = new tUITextDesc;
		pDesc->Color = get<2>(_Other.m_vecUIText[i])->Color;
		pDesc->Independent = get<2>(_Other.m_vecUIText[i])->Independent;
		pDesc->Rot = get<2>(_Other.m_vecUIText[i])->Rot;
		pDesc->Scale = get<2>(_Other.m_vecUIText[i])->Scale;
		m_vecUIText.push_back(make_tuple(get<0>(_Other.m_vecUIText[i]), get<1>(_Other.m_vecUIText[i]), pDesc));
	}
}

CUICom::~CUICom()
{
	for (auto& tuple : m_vecUIText)
		delete get<2>(tuple);
}

void CUICom::FinalTick()
{
	// UI 의 화면상 위치 계산 후 마우스 커서의 호버링 체크
	Vec4 wpos(Transform()->GetWorldTrans(), 1.f);
	Vec3 scale = Transform()->GetWorldScale();
	Vec2 cursor = CKeyMgr::GetInst()->GetMousePos();

	Vec4 vpos = XMVector4Transform(wpos, g_Trans.matView);	// 뷰 변환 적용 이후 사각형 확장으로 빌보딩
	Vec3 vposLT(vpos.x - scale.x * 0.5f, vpos.y + scale.y * 0.5f, vpos.z);
	Vec3 vposRB(vpos.x + scale.x * 0.5f, vpos.y - scale.y * 0.5f, vpos.z);

	Vec4 pposLT = XMVector3Transform(vposLT, g_Trans.matProj);
	Vec4 pposRB = XMVector3Transform(vposRB, g_Trans.matProj);

	Vec2 NDCLT(pposLT.x / pposLT.w, pposLT.y / pposLT.w);
	Vec2 NDCRB(pposRB.x / pposRB.w, pposRB.y / pposRB.w);

	m_ScreenLT = Vec2((NDCLT.x + 1.0f) * 0.5f * CEngine::GetInst()->GetResolution().x, (1.0f - NDCLT.y) * 0.5f * CEngine::GetInst()->GetResolution().y);
	m_ScreenRB = Vec2((NDCRB.x + 1.0f) * 0.5f * CEngine::GetInst()->GetResolution().x, (1.0f - NDCRB.y) * 0.5f * CEngine::GetInst()->GetResolution().y);

	if (m_ScreenLT.x < cursor.x && m_ScreenLT.y < cursor.y && m_ScreenRB.x > cursor.x && m_ScreenRB.y > cursor.y)
		m_MouseHover = true;
	else
		m_MouseHover = false;

	// 마우스 좌클릭 이벤트 체크
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::TAP && m_MouseHover)
		m_Clicked = true;

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::PRESSED && m_MouseHover && m_Clicked)
	{
		if (m_KeydownEvent)
			m_KeydownEvent(GetOwner());
	}

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::RELEASE && m_MouseHover && m_Clicked)
	{
		m_Clicked = false;
		if (m_ClickEvent)
			m_ClickEvent(GetOwner());
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

void CUICom::Render()
{
	// 텍스트 렌더링 (렌더 컴포넌트 호출 후 별도로 호출됨)
	Vec4 clip = XMVector3Transform(XMVector3Transform(Transform()->GetWorldTrans(), g_Trans.matView), g_Trans.matProj);
	float depth = ((clip.z / clip.w) + 1) * 0.5f;
	for (auto& tuple : m_vecUIText)
	{
		float scale = 1 - depth;
		if (get<2>(tuple)->Independent)
			scale = get<2>(tuple)->Scale;
		get<0>(tuple)->Render(get<1>(tuple), m_ScreenLT, get<2>(tuple)->Color, get<2>(tuple)->Rot, (m_ScreenLT + m_ScreenRB) / 2.f, scale, depth);
	}
}

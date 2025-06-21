#include "pch.h"
#include "CUICom.h"

#include "CKeyMgr.h"
#include "CEngine.h"
#include "CAssetMgr.h"

#include "CTransform.h"
#include "CFont.h"

CUICom::CUICom()
	:CRenderComponent(COMPONENT_TYPE::UICOM)
	, m_ClickEvent(nullptr)
	, m_KeydownEvent(nullptr)
	, m_MouseHover(false)
	, m_Clicked(false)
	, m_vecUIText{}
	, m_ScreenPosNDC()
	, m_Scale()
	, m_Active(false)
{
}

CUICom::CUICom(const CUICom& _Other)
	:CRenderComponent(_Other)
	, m_ClickEvent(_Other.m_ClickEvent)
	, m_KeydownEvent(_Other.m_KeydownEvent)
	, m_MouseHover(false)
	, m_Clicked(false)
	, m_vecUIText{}
	, m_ScreenPosNDC(_Other.m_ScreenPosNDC)
	, m_Scale(_Other.m_Scale)
	, m_Active(false)
{
	for (int i = 0; i < _Other.m_vecUIText.size(); ++i)
	{
		tUITextDesc* pDesc = new tUITextDesc;
		pDesc->Color = get<2>(_Other.m_vecUIText[i])->Color;
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
	// point mesh �� ����
	SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh", true));

	if (m_Active)
	{
		// UI �� ȭ��� ��ġ ��� �� ���콺 Ŀ���� ȣ���� üũ
		Vec2 cursor = CKeyMgr::GetInst()->GetMousePos();
		Vec2 resolution = CEngine::GetInst()->GetResolution();
		Vec2 ScreenPos(((m_ScreenPosNDC.x + 1) / 2.f) * resolution.x, ((1 - m_ScreenPosNDC.y) / 2.f) * resolution.y);

		Vec2 ScreenLT(ScreenPos - Vec2(m_Scale / 2.f));
		Vec2 ScreenRB(ScreenPos + Vec2(m_Scale / 2.f));

		if (ScreenLT.x < cursor.x && ScreenLT.y < cursor.y && ScreenRB.x > cursor.x && ScreenRB.y > cursor.y)
			m_MouseHover = true;
		else
			m_MouseHover = false;

		// ���콺 ��Ŭ�� �̺�Ʈ üũ
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
	else
	{
		m_MouseHover = false;
		m_Clicked = false;
	}


}

int CUICom::Save(fstream& _Stream)
{
	if (FAILED(CRenderComponent::RenderCom_Save(_Stream)))
		return E_FAIL;

	// �ؽ�Ʈ ���� ���� ����
	int count = m_vecUIText.size();
	_Stream.write(reinterpret_cast<char*>(&count), sizeof(int));

	for (int i = 0; i < count; ++i)
	{
		// ��Ʈ ������ �̸����� ����
		std::wstring fontName = get<0>(m_vecUIText[i])->GetName();
		int size = fontName.length();
		_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
		if (size > 0)
		{
			_Stream.write(reinterpret_cast<char*>(fontName.data()), sizeof(wchar_t) * size);
		}

		// �ؽ�Ʈ ����
		std::wstring text = get<1>(m_vecUIText[i]);
		size = text.length();
		_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
		if (size > 0)
		{
			_Stream.write(reinterpret_cast<char*>(text.data()), sizeof(wchar_t) * size);
		}

		// �ؽ�Ʈ ��ũ���� ����
		tUITextDesc* pTextDesc = get<2>(m_vecUIText[i]);
		_Stream.write(reinterpret_cast<char*>(pTextDesc), sizeof(tUITextDesc));
	}

	_Stream.write(reinterpret_cast<char*>(&m_ScreenPosNDC), sizeof(Vec2));
	_Stream.write(reinterpret_cast<char*>(&m_Scale), sizeof(Vec2));

	return S_OK;
}

int CUICom::Load(fstream& _Stream)
{
	if (FAILED(CRenderComponent::RenderCom_Load(_Stream)))
		return E_FAIL;

	// �ؽ�Ʈ ���� ���� �о����
	int count = 0;
	_Stream.read(reinterpret_cast<char*>(&count), sizeof(int));

	for (int i = 0; i < count; ++i)
	{
		// ����� �̸����� ��Ʈ ���� �ε�
		int size = 0;
		std::wstring fontName = {};
		AssetPtr<CFont> pFont = nullptr;
		_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
		if (size > 0)
		{
			fontName.resize(size);
			_Stream.read(reinterpret_cast<char*>(fontName.data()), sizeof(wchar_t) * size);
			pFont = CAssetMgr::GetInst()->Load<CFont>(fontName);
		}

		// ����� �ؽ�Ʈ �о����
		std::wstring text = {};
		_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
		if (size > 0)
		{
			text.resize(size);
			_Stream.read(reinterpret_cast<char*>(text.data()), sizeof(wchar_t) * size);
		}

		// ����� �ؽ�Ʈ ��ũ���� �о����
		tUITextDesc* pTextDesc = new tUITextDesc;
		_Stream.read(reinterpret_cast<char*>(pTextDesc), sizeof(tUITextDesc));

		// ��Ʈ�� ���������� �ε��� ��쿡�� �ؽ�Ʈ ���Ϳ� ����
		if (pFont.Get())
		{
			AddUIText(pFont, text, pTextDesc);
		}
	}

	// UI �� ��ũ���� ��ġ�� ������ �о����
	_Stream.read(reinterpret_cast<char*>(&m_ScreenPosNDC), sizeof(Vec2));
	_Stream.read(reinterpret_cast<char*>(&m_Scale), sizeof(Vec2));

	return S_OK;
}

void CUICom::Render()
{
	// ��ġ���� ������Ʈ
	Transform()->Binding();

	// ����� ���̴� ���ε�
	GetMaterial()->SetScalarParam(VEC2_0, m_ScreenPosNDC);
	GetMaterial()->SetScalarParam(FLOAT_1, m_Scale.x);
	GetMaterial()->SetScalarParam(FLOAT_2, m_Scale.y);
	GetMaterial()->Binding();

	// ������ ����
	GetMesh()->Render();

	// �ؽ�Ʈ ������ (���� ������Ʈ ȣ�� �� ������ ȣ���)
	//Vec4 clip = XMVector3Transform(XMVector3Transform(Transform()->GetWorldTrans(), g_Trans.matView), g_Trans.matProj);
	//float depth = ((clip.z / clip.w) + 1) * 0.5f;
	Vec2 resolution = CEngine::GetInst()->GetResolution();
	Vec2 ScreenPos(((m_ScreenPosNDC.x + 1) / 2.f) * resolution.x, ((1 - m_ScreenPosNDC.y) / 2.f) * resolution.y);
	//Vec2 ScreenLT(ScreenPos - m_Scale);
	for (auto& tuple : m_vecUIText)
	{
		get<0>(tuple)->Render(get<1>(tuple), ScreenPos, get<2>(tuple)->Color, get<2>(tuple)->Rot, get<2>(tuple)->Scale, 0);
	}
}

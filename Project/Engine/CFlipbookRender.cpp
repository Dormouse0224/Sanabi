#include "pch.h"
#include "CFlipbookRender.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"

#include "CTransform.h"
#include "CFlipbook.h"

CFlipbookRender::CFlipbookRender()
	: CRenderComponent(COMPONENT_TYPE::FLIPBOOKRENDER)
{
	SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh", true));

	//CreateFlipbookMaterial();
}

CFlipbookRender::~CFlipbookRender()
{
}

void CFlipbookRender::FinalTick()
{
	if (nullptr == m_CurFlipbook)
		return;

	if (m_Finish)
	{
		if (m_Repeat)
		{
			m_SceneIdx = 0;
			m_Finish = false;
		}
		else
		{
			return;
		}
	}

	m_Time += DT;

	if ((1.f / m_FPS) < m_Time)
	{
		m_Time -= (1.f / m_FPS);

		++m_SceneIdx;

		if (m_CurFlipbook->GetSceneCount() <= m_SceneIdx)
		{
			m_Finish = true;
			m_SceneIdx -= 1;
		}
	}
}

void CFlipbookRender::Render()
{
	if (nullptr == m_CurFlipbook)
		return;

	// ��ġ���� ������Ʈ
	Transform()->Binding();

	// �ø����� ������ �� Ÿ�Կ� ���� �ٸ� ���� ����
	if (m_CurFlipbook->GetSceneType() == SceneType::SPRITE)
	{
		// ���� �������� ������ ��������Ʈ�� ���
		AssetPtr<CSprite> pCurSprite = m_CurFlipbook->GetScene<CSprite>(m_SceneIdx);
		AssetPtr<CTexture2D> pAtlas = pCurSprite->GetAtlasTex();
		Vec2 vLeftTop = pCurSprite->GetLeftTop();
		Vec2 vSlice = pCurSprite->GetSlice();
		Vec2 vBackground = pCurSprite->GetBackground();
		Vec2 vOffset = pCurSprite->GetOffset();


		// ����� ���̴� ���ε�	
		if (nullptr != pCurSprite)
		{
			GetMaterial()->SetScalarParam(INT_0, (int)!!pCurSprite->GetAtlasTex().Get());
			GetMaterial()->SetTexParam(TEX_0, pAtlas);
			GetMaterial()->SetScalarParam(VEC2_0, vLeftTop);
			GetMaterial()->SetScalarParam(VEC2_1, vSlice);
			GetMaterial()->SetScalarParam(VEC2_2, vBackground);
			GetMaterial()->SetScalarParam(VEC2_3, vOffset);
		}
		else
		{
			GetMaterial()->SetScalarParam(INT_0, 0);
			GetMaterial()->SetTexParam(TEX_0, nullptr);
		}
	}
	else if (m_CurFlipbook->GetSceneType() == SceneType::TEXTURE)
	{
		AssetPtr<CTexture2D> pCurTex = m_CurFlipbook->GetScene<CTexture2D>(m_SceneIdx);

		GetMaterial()->SetScalarParam(INT_0, 0);
		GetMaterial()->SetScalarParam(INT_1, (int)!!pCurTex.Get());
		GetMaterial()->SetTexParam(TEX_0, pCurTex);
	}

	GetMaterial()->Binding();

	// ������ ����
	GetMesh()->Render();
}

int CFlipbookRender::Load(fstream& _Stream)
{
	if (FAILED(CRenderComponent::RenderCom_Load(_Stream)))
		return E_FAIL;

	int count = 0;
	_Stream.read(reinterpret_cast<char*>(&count), sizeof(int));
	for (int i = 0; i < count; ++i)
	{
		std::wstring FlipbookName = {};
		int size = 0;
		_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
		FlipbookName.resize(size);
		_Stream.read(reinterpret_cast<char*>(FlipbookName.data()), sizeof(wchar_t) * size);
		m_vecFlipbook.push_back(CAssetMgr::GetInst()->Load<CFlipbook>(FlipbookName));
	}

	return S_OK;
}

int CFlipbookRender::Save(fstream& _Stream)
{
	// ��� ������ Play �� ȣ��� �� �Է¹����Ƿ� �������� �ʴ´�.
	if (FAILED(CRenderComponent::RenderCom_Save(_Stream)))
		return E_FAIL;

	int count = m_vecFlipbook.size();
	_Stream.write(reinterpret_cast<char*>(&count), sizeof(int));
	for (int i = 0; i < count; ++i)
	{
		std::wstring FlipbookName = m_vecFlipbook[i]->GetName();
		int size = FlipbookName.size();
		_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
		_Stream.write(reinterpret_cast<char*>(FlipbookName.data()), sizeof(wchar_t) * size);
	}

	return S_OK;
}

//void CFlipbookRender::CreateFlipbookMaterial()
//{
//	if (nullptr == CAssetMgr::GetInst()->Load<CGraphicShader>(L"EA_FlipbookShader", true))
//	{
//		// FlipbookShader
//		AssetPtr<CGraphicShader> pShader = new CGraphicShader;
//		pShader->CreateVertexShader(L"HLSL\\Engine\\flipbook.fx", "VS_Flipbook");
//		pShader->CreatePixelShader(L"HLSL\\Engine\\flipbook.fx", "PS_Flipbook");
//		pShader->SetRSType(RS_TYPE::CULL_NONE);
//		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
//		pShader->SetTexData(TEX_0, "Atlas Texture");
//		pShader->SetConstData(INT_0, "Atlas Tex Flag");
//		pShader->SetConstData(INT_1, "Single Tex Flag");
//		pShader->SetConstData(VEC2_0, "LeftTop");
//		pShader->SetConstData(VEC2_1, "Slice");
//		pShader->SetConstData(VEC2_2, "Background");
//		pShader->SetConstData(VEC2_3, "Offset");
//		CAssetMgr::GetInst()->AddAsset(L"FlipbookShader", pShader.Get());
//	}
//
//	if (nullptr == CAssetMgr::GetInst()->Load<CMaterial>(L"FlipbookMtrl", true))
//	{
//		// FlipbookMtrl
//		AssetPtr<CMaterial> pMtrl = new CMaterial;
//		pMtrl->SetShader(CAssetMgr::GetInst()->Load<CGraphicShader>(L"FlipbookShader", true));
//		CAssetMgr::GetInst()->AddAsset(L"FlipbookMtrl", pMtrl.Get());
//	}
//
//	SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"FlipbookMtrl", true));
//}

void CFlipbookRender::AddFlipbook(AssetPtr<CFlipbook> _Flipbook)
{
	//if (m_vecFlipbook.size() <= _Idx)
	//{
	//	m_vecFlipbook.resize(_Idx + 1);
	//}
	if (_Flipbook.Get())
		m_vecFlipbook.push_back(_Flipbook);
}

void CFlipbookRender::DeleteFlipbook(int _FlipIdx)
{
	if (_FlipIdx >= m_vecFlipbook.size())
		return;

	m_vecFlipbook.erase(m_vecFlipbook.begin() + _FlipIdx);
}
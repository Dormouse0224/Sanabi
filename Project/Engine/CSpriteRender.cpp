#include "pch.h"
#include "CSpriteRender.h"

#include "CAssetMgr.h"
#include "CTransform.h"

CSpriteRender::CSpriteRender()
	: CRenderComponent(COMPONENT_TYPE::SPRITERENDER)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	CreateSpriteMaterial();
}

CSpriteRender::~CSpriteRender()
{
}


void CSpriteRender::FinalTick()
{
}

void CSpriteRender::Render()
{
	// ��ġ���� ������Ʈ
	Transform()->Binding();

	// ����� ���̴� ���ε�	
	if (nullptr != m_Sprite)
	{
		GetMaterial()->SetScalarParam(INT_0, (int)!!m_Sprite->GetAtlasTex().Get());
		GetMaterial()->SetTexParam(TEX_0, m_Sprite->GetAtlasTex());
		GetMaterial()->SetScalarParam(VEC2_0, m_Sprite->GetLeftTop());
		GetMaterial()->SetScalarParam(VEC2_1, m_Sprite->GetSlice());
	}
	else
	{
		GetMaterial()->SetScalarParam(INT_0, 0);
		GetMaterial()->SetTexParam(TEX_0, nullptr);
	}

	GetMaterial()->Binding();

	// ������ ����
	GetMesh()->Render();
}

int CSpriteRender::Load(fstream& _Stream)
{
	if (FAILED(CRenderComponent::RenderCom_Load(_Stream)))
		return E_FAIL;

	std::wstring SpriteName = {};
	int size = 0;
	_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
	SpriteName.resize(size);
	_Stream.read(reinterpret_cast<char*>(SpriteName.data()), sizeof(wchar_t) * size);
	m_Sprite = CAssetMgr::GetInst()->Load<CSprite>(SpriteName);

	return S_OK;
}

int CSpriteRender::Save(fstream& _Stream)
{
	if (FAILED(CRenderComponent::RenderCom_Save(_Stream)))
		return E_FAIL;

	std::wstring SpriteName = m_Sprite->GetName();
	int size = SpriteName.size();
	_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
	_Stream.write(reinterpret_cast<char*>(SpriteName.data()), sizeof(wchar_t) * size);

	return S_OK;
}

void CSpriteRender::CreateSpriteMaterial()
{
	if (nullptr == CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"SpriteShader"))
	{
		// SpriteShader
		AssetPtr<CGraphicShader> pShader = new CGraphicShader;
		pShader->CreateVertexShader(L"HLSL\\Engine\\sprite.fx", "VS_Sprite");
		pShader->CreatePixelShader(L"HLSL\\Engine\\sprite.fx", "PS_Sprite");
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		CAssetMgr::GetInst()->AddAsset(L"SpriteShader", pShader.Get());
	}

	if (nullptr == CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpriteMtrl"))
	{
		// SpriteMtrl
		AssetPtr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"SpriteShader"));
		CAssetMgr::GetInst()->AddAsset(L"SpriteMtrl", pMtrl.Get());
	}

	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpriteMtrl"));
}
#include "pch.h"
#include "CTileRender.h"

#include "CAssetMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"

CTileRender::CTileRender()
	: CRenderComponent(COMPONENT_TYPE::TILERENDER)
	, m_Col(0)
	, m_Row(0)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	CreateTileRenderMtrl();

	m_GpuBuffer = new CStructuredBuffer;
}

CTileRender::~CTileRender()
{
	Delete_Vec(m_vecTileInfo);

	if (nullptr != m_GpuBuffer)
		delete m_GpuBuffer;
}


void CTileRender::FinalTick()
{

}

void CTileRender::Render()
{
	// 위치정보 업데이트
	Transform()->Binding();

	GetMaterial()->SetScalarParam(INT_0, (int)!!m_vecTileInfo[0]->Sprite->GetAtlasTex().Get());
	GetMaterial()->SetScalarParam(INT_1, m_Col);
	GetMaterial()->SetScalarParam(INT_2, m_Row);

	GetMaterial()->SetTexParam(TEX_0, GetAtlasTex());
	GetMaterial()->SetScalarParam(VEC2_0, m_vecTileInfo[0]->Sprite->GetLeftTop());
	GetMaterial()->SetScalarParam(VEC2_1, m_vecTileInfo[0]->Sprite->GetSlice());

	GetMaterial()->Binding();

	// 타일정보를 저장하고있는 구조화버퍼 바인딩
	m_GpuBuffer->Binding(20);

	// 렌더링 시작
	GetMesh()->Render();
}


void CTileRender::SetColRow(UINT _Col, UINT _Row)
{
	m_Col = _Col;
	m_Row = _Row;

	if ((m_Col * m_Row) < m_vecTileInfo.capacity())
	{
		vector<tTileInfo*> temp;
		m_vecTileInfo.swap(temp);
		Delete_Vec(temp);
	}

	// 벡터크기를 타일 개수만큼 재 설정
	m_vecTileInfo.resize(m_Col * m_Row);

	// 각 포인터가 타일정보를 가질 수 있게 함
	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		m_vecTileInfo[i] = new tTileInfo;
	}

	// 오브젝트 스케일 재 조정
	Vec2 vScale = m_TileSize * Vec2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vec3(vScale.x, vScale.y, 1.f));

	// 타일 데이터를 버퍼로 전송
	UpdateBuffer();
}

void CTileRender::SetTileSize(Vec2 _TileSize)
{
	m_TileSize = _TileSize;

	// 오브젝트 스케일 재 조정
	Vec2 vScale = m_TileSize * Vec2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vec3(vScale.x, vScale.y, 1.f));
}

void CTileRender::SetSprite(UINT _Col, UINT _Row, AssetPtr<CSprite> _Sprite)
{
	assert(m_Col > _Col && m_Row > _Row);

	int idx = m_Col * _Row + _Col;
	m_vecTileInfo[idx]->Sprite = _Sprite;

	// 타일 데이터를 버퍼로 전송
	UpdateBuffer();
}

void CTileRender::CreateTileRenderMtrl()
{
	if (nullptr == CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"TileRenderShader"))
	{
		// TileRenderShader
		AssetPtr<CGraphicShader> pShader = new CGraphicShader;
		pShader->CreateVertexShader(L"HLSL\\Engine\\tilerender.fx", "VS_TileRender");
		pShader->CreatePixelShader(L"HLSL\\Engine\\tilerender.fx", "PS_TileRender");
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		CAssetMgr::GetInst()->AddAsset(L"TileRenderShader", pShader.Get());
	}

	if (nullptr == CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileRenderMtrl"))
	{
		// TileRenderMtrl
		AssetPtr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"TileRenderShader"));
		CAssetMgr::GetInst()->AddAsset(L"TileRenderMtrl", pMtrl.Get());
	}

	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileRenderMtrl"));
}


AssetPtr<CTexture2D> CTileRender::GetAtlasTex()
{
	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		if (nullptr == m_vecTileInfo[i]->Sprite)
			continue;

		return m_vecTileInfo[i]->Sprite->GetAtlasTex();
	}

	return nullptr;
}

void CTileRender::UpdateBuffer()
{
	if (m_GpuBuffer->GetElementCount() < m_Row * m_Col)
	{
		m_GpuBuffer->Create(16, m_Row * m_Col, SB_TYPE::SRV_ONLY, true);
	}

	struct SpriteInfo {
		Vec2 vLeftTop;
		Vec2 vSlice;
	};
	static vector<SpriteInfo> vecInfo;
	vecInfo.clear();

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		SpriteInfo info;
		if (m_vecTileInfo[i]->Sprite.Get())
		{
			info.vLeftTop = m_vecTileInfo[i]->Sprite->GetLeftTop();
			info.vSlice = m_vecTileInfo[i]->Sprite->GetSlice();
		}
		else
		{
			info.vLeftTop = Vec2(-1.f, -1.f);
			info.vSlice = Vec2(-1.f, -1.f);
		}

		vecInfo.push_back(info);
	}

	m_GpuBuffer->SetData(vecInfo.data(), sizeof(SpriteInfo), vecInfo.size());
}
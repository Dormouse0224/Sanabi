#include "pch.h"
#include "CTileRender.h"

#include "CAssetMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"

CTileRender::CTileRender()
	: CRenderComponent(COMPONENT_TYPE::TILERENDER)
	, m_Col(0)
	, m_Row(0)
	, m_TileSize()
	, m_AtlasTex()
	, m_vecTileLT{}
	, m_GpuBuffer(nullptr)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	CreateTileRenderMtrl();

	m_GpuBuffer = new CStructuredBuffer;
}

CTileRender::CTileRender(const CTileRender& _Other)
	: CRenderComponent(_Other)
	, m_Col(_Other.m_Col)
	, m_Row(_Other.m_Row)
	, m_TileSize(_Other.m_TileSize)
	, m_AtlasTex(_Other.m_AtlasTex)
	, m_vecTileLT{ _Other.m_vecTileLT }
	, m_GpuBuffer(nullptr)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	CreateTileRenderMtrl();

	m_GpuBuffer = new CStructuredBuffer;
}

CTileRender::~CTileRender()
{
	if (nullptr != m_GpuBuffer)
		delete m_GpuBuffer;
}


void CTileRender::FinalTick()
{

}

void CTileRender::Render()
{
	// ��ġ���� ������Ʈ
	Transform()->Binding();

	GetMaterial()->SetScalarParam(INT_0, m_Col);
	GetMaterial()->SetScalarParam(INT_1, m_Row);

	GetMaterial()->SetTexParam(TEX_0, m_AtlasTex);

	GetMaterial()->SetScalarParam(VEC2_0, m_TileSize);

	GetMaterial()->Binding();

	// Ÿ�������� �����ϰ��ִ� ����ȭ���� ���ε�
	m_GpuBuffer->Binding(20);

	// ������ ����
	GetMesh()->Render();
}

int CTileRender::Load(fstream& _Stream)
{
	if (FAILED(CRenderComponent::RenderCom_Save(_Stream)))
		return E_FAIL;


	_Stream.read(reinterpret_cast<char*>(&m_Col), sizeof(UINT));
	_Stream.read(reinterpret_cast<char*>(&m_Row), sizeof(UINT));
	_Stream.read(reinterpret_cast<char*>(&m_TileSize), sizeof(Vec2));

	std::wstring AtlasName = {};
	int size = 0;
	_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
	AtlasName.resize(size);
	_Stream.read(reinterpret_cast<char*>(AtlasName.data()), sizeof(wchar_t) * size);
	m_AtlasTex = CAssetMgr::GetInst()->Load<CTexture2D>(AtlasName);

	int count = 0;
	_Stream.read(reinterpret_cast<char*>(&count), sizeof(int));
	m_vecTileLT.resize(count);
	for (int i = 0; i < count; ++i)
	{
		_Stream.read(reinterpret_cast<char*>(&m_vecTileLT[i]), sizeof(Vec2));
	}

	return S_OK;
}

int CTileRender::Save(fstream& _Stream)
{
	if (FAILED(CRenderComponent::RenderCom_Save(_Stream)))
		return E_FAIL;


	_Stream.write(reinterpret_cast<char*>(&m_Col), sizeof(UINT));
	_Stream.write(reinterpret_cast<char*>(&m_Row), sizeof(UINT));
	_Stream.write(reinterpret_cast<char*>(&m_TileSize), sizeof(Vec2));

	std::wstring AtlasName = {};
	if (m_AtlasTex.Get())
		AtlasName = m_AtlasTex->GetName();
	int size = AtlasName.size();
	_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
	_Stream.write(reinterpret_cast<char*>(AtlasName.data()), sizeof(wchar_t) * size);

	int count = m_vecTileLT.size();
	_Stream.write(reinterpret_cast<char*>(&count), sizeof(int));
	for (int i = 0; i < count; ++i)
	{
		_Stream.write(reinterpret_cast<char*>(&m_vecTileLT[i]), sizeof(Vec2));
	}

	return S_OK;
}


void CTileRender::SetColRow(UINT _Col, UINT _Row)
{
	m_Col = _Col;
	m_Row = _Row;

	// ���� ũ�Ⱑ Ÿ�� �������� ū ��� ���� �޸� ����
	if ((m_Col * m_Row) < m_vecTileLT.capacity())
	{
		vector<Vec2> temp;
		m_vecTileLT.swap(temp);
	}

	// ���� �޸� ũ�⸦ Ÿ�� ������ŭ �缳��
	m_vecTileLT.resize(m_Col * m_Row);

	// ���� �� ��� LT ���� (-1, -1) �� �ʱ�ȭ (�ش� �ε������� Ÿ���� ���ٴ� ��)
	for (auto& LT : m_vecTileLT)
		LT = Vec2(-1, -1);

	// ������Ʈ ������ �� ����
	Vec2 vScale = m_TileSize * Vec2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vec3(vScale.x, vScale.y, 1.f));

	// Ÿ�� �����͸� ���ۿ� ����ȭ
	UpdateBuffer();
}

void CTileRender::SetTileSize(Vec2 _TileSize)
{
	m_TileSize = _TileSize;

	// ������Ʈ ������ �� ����
	Vec2 vScale = m_TileSize * Vec2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vec3(vScale.x, vScale.y, 1.f));
}

void CTileRender::SetTile(UINT _Col, UINT _Row, Vec2 _LT)
{
	assert(m_Col > _Col && m_Row > _Row);

	int idx = m_Col * _Row + _Col;
	m_vecTileLT[idx] = _LT;

	// Ÿ�� �����͸� ���ۿ� ����ȭ
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

void CTileRender::UpdateBuffer()
{
	struct SpriteInfo {
		Vec2 vLeftTop;
	};

	if (m_GpuBuffer->GetElementCount() < m_Row * m_Col)
	{
		m_GpuBuffer->Create(sizeof(SpriteInfo), m_Row* m_Col, SB_TYPE::SRV_ONLY, true);
	}

	
	static vector<SpriteInfo> vecInfo;
	vecInfo.clear();

	for (size_t i = 0; i < m_vecTileLT.size(); ++i)
	{
		SpriteInfo info;
		info.vLeftTop = m_vecTileLT[i];
		vecInfo.push_back(info);
	}

	m_GpuBuffer->SetData(vecInfo.data(), sizeof(SpriteInfo), vecInfo.size());
}
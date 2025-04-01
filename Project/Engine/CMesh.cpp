#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"

CMesh::CMesh()
	: CAsset(ASSET_TYPE::MESH)
	, m_VBDesc{}
	, m_IBDesc{}
	, m_VtxSysMem(nullptr)
	, m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (nullptr != m_VtxSysMem)
		delete m_VtxSysMem;

	if (nullptr != m_IdxSysMem)
		delete m_IdxSysMem;
}

int CMesh::Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount)
{
	m_VtxCount = _VtxCount;
	m_IdxCount = _IdxCount;


	// ���� ũ�� ����
	m_VBDesc.ByteWidth = sizeof(Vtx) * _VtxCount;

	// ���� ���Ŀ� ������ �̵� ����
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA Sub = {};
	Sub.pSysMem = _VtxSysMem;

	DEVICE->CreateBuffer(&m_VBDesc, &Sub, m_VB.GetAddressOf());

	// ���� ũ�� ����
	m_IBDesc.ByteWidth = sizeof(UINT) * _IdxCount;

	// ���� ���Ŀ� ������ �̵� ����
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	Sub.pSysMem = _IdxSysMem;

	DEVICE->CreateBuffer(&m_IBDesc, &Sub, m_IB.GetAddressOf());


	// System �޸𸮿� ������ �ε��� ���� ���
	m_VtxSysMem = new Vtx[m_VtxCount];
	m_IdxSysMem = new UINT[m_IdxCount];

	memcpy(m_VtxSysMem, _VtxSysMem, sizeof(Vtx) * m_VtxCount);
	memcpy(m_IdxSysMem, _IdxSysMem, sizeof(UINT) * m_IdxCount);

	DEBUG_NAME(m_VB, "CMesh_VB");
	DEBUG_NAME(m_IB, "CMesh_IB");

	return S_OK;
}


void CMesh::Binding()
{
	UINT Stride = sizeof(Vtx);
	UINT Offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &Stride, &Offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render()
{
	Binding();

	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void CMesh::Render_Particle(UINT _Count)
{
	Binding();

	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0);
}

int CMesh::Save(const wstring& _FileName)
{
	std::filesystem::path path = CPathMgr::GetContentDir() + std::wstring(L"Mesh\\") + _FileName + std::wstring(L".mesh");
	if (!std::filesystem::exists(path.parent_path()))
		std::filesystem::create_directories(path.parent_path());
	std::fstream file(path, std::ios::out | std::ios::binary);

	// ���� ���� ���� ����
	file.write(reinterpret_cast<char*>(&m_VBDesc), sizeof(D3D11_BUFFER_DESC));
	file.write(reinterpret_cast<char*>(&m_VtxCount), sizeof(UINT));
	if (m_VtxSysMem)
		file.write(reinterpret_cast<char*>(m_VtxSysMem), sizeof(Vtx) * m_VtxCount);

	// �ε��� ���� ���� ����
	file.write(reinterpret_cast<char*>(&m_IBDesc), sizeof(D3D11_BUFFER_DESC));
	file.write(reinterpret_cast<char*>(&m_IdxCount), sizeof(UINT));
	if (m_IdxSysMem)
		file.write(reinterpret_cast<char*>(m_IdxSysMem), sizeof(UINT) * m_IdxCount);

	file.close();
	return S_OK;
}

int CMesh::Load(const wstring& _FilePath)
{
	std::fstream file(CPathMgr::GetContentDir() + _FilePath, std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		MessageBoxW(nullptr, L"������ ã�� �� �����ϴ�.\n��θ� Ȯ���� �ּ���.", L"Mesh Load Error", MB_OK);
		return E_FAIL;
	}

	// ���� ���� ���� �ҷ�����
	file.read((char*)&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	file.read((char*)&m_VtxCount, sizeof(UINT));
	if (m_VtxCount != 0)
	{
		delete[] m_VtxSysMem;
		m_VtxSysMem = new Vtx[m_VtxCount];
		file.read((char*)m_VtxSysMem, sizeof(Vtx) * m_VtxCount);
	}

	// �ε��� ���� ���� �ҷ�����
	file.read((char*)&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	file.read((char*)&m_IdxCount, sizeof(UINT));
	if (m_IdxCount != 0)
	{
		delete[] m_IdxSysMem;
		m_IdxSysMem = new UINT[m_IdxCount];
		file.read((char*)m_IdxSysMem, sizeof(UINT) * m_IdxCount);
	}

	file.close();

	// ���� ���� ����
	D3D11_SUBRESOURCE_DATA Sub = {};
	Sub.pSysMem = m_VtxSysMem;
	DEVICE->CreateBuffer(&m_VBDesc, &Sub, m_VB.GetAddressOf());

	// �ε��� ���� ����
	Sub = {};
	Sub.pSysMem = m_IdxSysMem;
	DEVICE->CreateBuffer(&m_IBDesc, &Sub, m_IB.GetAddressOf());

	DEBUG_NAME(m_VB, "CMesh_VB");
	DEBUG_NAME(m_IB, "CMesh_IB");

	CAssetMgr::GetInst()->AddAsset(_FilePath, this);

	return S_OK;
}
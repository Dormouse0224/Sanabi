#include "pch.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
{
}

CRenderComponent::~CRenderComponent()
{
}

int CRenderComponent::RenderCom_Load(fstream& _Stream)
{
	if (!_Stream.is_open())
		return E_FAIL;

	std::wstring MeshName = {};
	int size = 0;
	_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
	MeshName.resize(size);
	_Stream.read(reinterpret_cast<char*>(MeshName.data()), sizeof(wchar_t) * size);
	m_Mesh = CAssetMgr::GetInst()->Load<CMesh>(MeshName);

	std::wstring MtrlName = {};
	size = 0;
	_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
	MtrlName.resize(size);
	_Stream.read(reinterpret_cast<char*>(MtrlName.data()), sizeof(wchar_t) * size);
	m_Mtrl = CAssetMgr::GetInst()->Load<CMaterial>(MtrlName);

	return S_OK;
}

int CRenderComponent::RenderCom_Save(fstream& _Stream)
{
	if (!_Stream.is_open())
		return E_FAIL;

	std::wstring MeshName = m_Mesh->GetName();
	int size = MeshName.size();
	_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
	_Stream.write(reinterpret_cast<char*>(MeshName.data()), sizeof(wchar_t) * size);

	std::wstring MtrlName = m_Mtrl->GetName();
	size = MtrlName.size();
	_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
	_Stream.write(reinterpret_cast<char*>(MtrlName.data()), sizeof(wchar_t) * size);

	return S_OK;
}

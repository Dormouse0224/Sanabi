#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CLevel.h"

CPrefab::CPrefab()
	: CAsset(ASSET_TYPE::PREFAB)
{
}

CPrefab::~CPrefab()
{
	delete m_PrefabObject;
}

void CPrefab::ConvertToPrefab(CGameObject* _Obj)
{
	m_PrefabObject = _Obj->Clone();
	CAssetMgr::GetInst()->AddAsset(_Obj->GetName(), this);
}

CGameObject* CPrefab::Instantiate()
{
	return m_PrefabObject->Clone();
}

int CPrefab::Save(const wstring& _FileName)
{
	std::filesystem::path path = CPathMgr::GetContentDir() + std::wstring(L"Prefab\\") + _FileName + std::wstring(L".prefab");
	CPathMgr::CreateParentDir(path);
	std::fstream file(path, std::ios::out | std::ios::binary);

	if (FAILED(m_PrefabObject->Save(file)))
		return E_FAIL;

	file.close();
	return S_OK;
}

int CPrefab::Load(const wstring& _FilePath)
{
	std::filesystem::path path = CPathMgr::GetContentDir() + _FilePath;
	std::fstream file(path, std::ios::in | std::ios::binary);

	m_PrefabObject = new CGameObject;
	if (FAILED(m_PrefabObject->Load(file)))
		return E_FAIL;

	file.close();
	return S_OK;
}

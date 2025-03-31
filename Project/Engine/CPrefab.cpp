#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

CPrefab::CPrefab()
	: CAsset(ASSET_TYPE::PREFAB)
{
}

CPrefab::~CPrefab()
{
}

void CPrefab::SaveAsPrefab(CGameObject* _Obj)
{
	m_PrefabObject = _Obj;
}

CGameObject* CPrefab::Instantiate()
{
	return m_PrefabObject->Clone();
}

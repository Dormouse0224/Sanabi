#pragma once

#include "CAsset.h"
#include "assets.h"
#include "CPathMgr.h"

class CAssetMgr
	: public Singleton<CAssetMgr>
{
	SINGLE(CAssetMgr)
private:
	map<wstring, Ptr<CAsset>>	m_mapAsset[ASSET_END];

public:
	void Init();
	void CreateEngineMesh();
	void CreateEngineTexture();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();
	void CreateEngineMaterial();
	void CreateEngineSprite();

public:
	void AddAsset(const wstring& _Key, Ptr<CAsset> _Asset);

	template<typename T>
	Ptr<T> FindAsset(const wstring& _Key);

	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);

	// _BindFlag : D3D11_BIND_FLAG
	Ptr<CTexture> CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
};

template<typename T>
ASSET_TYPE GetAssetType()
{
	if constexpr (is_same_v<CMesh, T>)
		return MESH;
	else if constexpr (is_same_v<CTexture, T>)
		return TEXTURE;
	else if constexpr (is_same_v<CMaterial, T>)
		return MATERIAL;
	else if constexpr (is_same_v<CGraphicShader, T>)
		return GRAPHIC_SHADER;
	else if constexpr (is_same_v<CComputeShader, T>)
		return COMPUTE_SHADER;
	else if constexpr (is_same_v<CSprite, T>)
		return SPRITE;
	else if constexpr (is_same_v<CFlipbook, T>)
		return FLIPBOOK;
}

template<typename T>
inline Ptr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[Type].find(_Key);

	if (iter == m_mapAsset[Type].end())
		return nullptr;

	return (T*)iter->second.Get();
}

template<typename T>
inline Ptr<T> CAssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	Ptr<T> pAsset = FindAsset<T>(_Key);

	if (nullptr != pAsset)
		return pAsset;

	wstring ContentPath = CPathMgr::GetContentPath();

	pAsset = new T;
	if (FAILED(pAsset->Load(ContentPath + _RelativePath)))
	{
		MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 에러", MB_OK);
		return nullptr;
	}

	AddAsset(_Key, pAsset.Get());


	return pAsset;
}

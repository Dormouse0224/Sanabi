#pragma once

#include "CAsset.h"
#include "assets.h"
#include "CPathMgr.h"

class CAssetMgr
	: public Singleton<CAssetMgr>
{
	SINGLE(CAssetMgr)
private:
	map<wstring, AssetPtr<CAsset>>	m_mapAsset[ASSET_END];

public:
	void Init();
	void CreateEngineMesh();
	void CreateEngineTexture();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();
	void CreateEngineMaterial();
	void CreateEngineSprite();

public:
	void AddAsset(const wstring& _Key, AssetPtr<CAsset> _Asset);

	template<typename T>
	AssetPtr<T> FindAsset(const wstring& _RelativePath);

	template<typename T>
	AssetPtr<T> Load(const wstring& _RelativePath);

	// _BindFlag : D3D11_BIND_FLAG
	AssetPtr<CTexture> CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
	AssetPtr<CTexture> CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D);
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
inline AssetPtr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, AssetPtr<CAsset>>::iterator iter = m_mapAsset[Type].find(_Key);

	if (iter == m_mapAsset[Type].end())
		return nullptr;

	return (T*)iter->second.Get();
}

template<typename T>
inline AssetPtr<T> CAssetMgr::Load(const wstring& _RelativePath)
{
	AssetPtr<T> pAsset = FindAsset<T>(_RelativePath);

	if (nullptr != pAsset)
		return pAsset;

	wstring ContentPath = CPathMgr::GetContentPath();

	pAsset = new T;
	if (FAILED(pAsset->Load(ContentPath + _RelativePath)))
	{
		MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 에러", MB_OK);
		return nullptr;
	}

	AddAsset(_RelativePath, pAsset.Get());


	return pAsset;
}

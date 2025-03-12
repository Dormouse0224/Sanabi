#pragma once

#include "CAsset.h"
#include "assets.h"
#include "CPathMgr.h"

class CAssetMgr
	: public Singleton<CAssetMgr>
{
	SINGLE(CAssetMgr)
private:
	map<wstring, AssetPtr<CAsset>>	m_mapAsset[(UINT)ASSET_TYPE::ASSET_END];
	bool m_Renew;

public:
	void Init();
	void Tick();
	void CreateEngineMesh();
	void CreateEngineTexture();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();
	void CreateEngineMaterial();
	void CreateEngineSprite();

public:
	template<typename T>
	ASSET_TYPE GetAssetType();
	const map<wstring, AssetPtr<CAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }
	bool GetRenew() { return m_Renew; }

	void AddAsset(const wstring& _Key, AssetPtr<CAsset> _Asset);
	template<typename T>
	AssetPtr<T> FindAsset(const wstring& _RelativePath);
	template<typename T>
	AssetPtr<T> Load(const wstring& _RelativePath);
	/// <param name="_BindFlag">flags of D3D11_BIND_FLAG</param>
	AssetPtr<CTexture> CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
	AssetPtr<CTexture> CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D);


};


template<typename T>
ASSET_TYPE CAssetMgr::GetAssetType()
{
	if constexpr (is_same_v<CMesh, T>)
		return ASSET_TYPE::MESH;
	else if constexpr (is_same_v<CTexture, T>)
		return ASSET_TYPE::TEXTURE;
	else if constexpr (is_same_v<CMaterial, T>)
		return ASSET_TYPE::MATERIAL;
	else if constexpr (is_same_v<CGraphicShader, T>)
		return ASSET_TYPE::GRAPHIC_SHADER;
	else if constexpr (is_same_v<CComputeShader, T>)
		return ASSET_TYPE::COMPUTE_SHADER;
	else if constexpr (is_same_v<CSprite, T>)
		return ASSET_TYPE::SPRITE;
	else if constexpr (is_same_v<CFlipbook, T>)
		return ASSET_TYPE::FLIPBOOK;
}

template<typename T>
inline AssetPtr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, AssetPtr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	if (iter == m_mapAsset[(UINT)Type].end())
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
#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"

CAssetMgr::CAssetMgr()
{

}

CAssetMgr::~CAssetMgr()
{

}

void CAssetMgr::Tick()
{
	m_Renew = false;
}

void CAssetMgr::AddAsset(const wstring& _Key, AssetPtr<CAsset> _Asset)
{
	ASSET_TYPE Type = _Asset->GetType();

	map<wstring, AssetPtr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	assert(iter == m_mapAsset[(UINT)Type].end());

	_Asset->m_Key = _Key;
	_Asset->SetName(_Key);
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, _Asset));
	m_Renew = true;
}

AssetPtr<CTexture2D> CAssetMgr::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height
	, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	AssetPtr<CTexture2D> pTex = FindAsset<CTexture2D>(_Key);
	assert(nullptr == pTex);	// 키값이 중복된 경우 중단합니다.

	pTex = new CTexture2D;
	if (FAILED(pTex->Create(_Width, _Height, _Format, _BindFlag, _Usage)))
	{
		MessageBox(nullptr, _Key.c_str(), L"텍스쳐 생성 실패", MB_OK);
		return nullptr;
	}

	AddAsset(_Key, pTex.Get());

	return pTex;
}

AssetPtr<CTexture2D> CAssetMgr::CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D)
{
	AssetPtr<CTexture2D> pTex = FindAsset<CTexture2D>(_Key);
	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture2D;
	if (FAILED(pTex->Create(_Tex2D)))
	{
		MessageBox(nullptr, _Key.c_str(), L"텍스쳐 생성 실패", MB_OK);
		return nullptr;
	}

	AddAsset(_Key, pTex.Get());

	return pTex;
}


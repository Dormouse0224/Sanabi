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

AssetPtr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height
	, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	AssetPtr<CTexture> pTex = FindAsset<CTexture>(_Key);
	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Width, _Height, _Format, _BindFlag, _Usage)))
	{
		MessageBox(nullptr, _Key.c_str(), L"텍스쳐 생성 실패", MB_OK);
		return nullptr;
	}

	AddAsset(_Key, pTex.Get());

	return pTex;
}

AssetPtr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D)
{
	AssetPtr<CTexture> pTex = FindAsset<CTexture>(_Key);
	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Tex2D)))
	{
		MessageBox(nullptr, _Key.c_str(), L"텍스쳐 생성 실패", MB_OK);
		return nullptr;
	}

	AddAsset(_Key, pTex.Get());

	return pTex;
}


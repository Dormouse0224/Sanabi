#pragma once

#include "CAsset.h"
#include "assets.h"
#include "CPathMgr.h"
#include "CEngine.h"

class CAssetMgr
	: public Singleton<CAssetMgr>
{
	SINGLE(CAssetMgr)
private:
	map<wstring, AssetPtr<CAsset>>	m_mapAsset[(UINT)ASSET_TYPE::ASSET_END];
	bool m_AssetModified;
	bool m_Renew;
	HANDLE m_DirNotifyHandle;

public:
	void Init();
	void Tick();

private:
	void CreateEngineMesh();
	void CreateEngineGraphicShader();

public:
	template<typename T>
	ASSET_TYPE GetAssetType();
	const map<wstring, AssetPtr<CAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }
	bool GetRenew() { return m_Renew; }

	void AddAsset(const wstring& _Key, AssetPtr<CAsset> _Asset);
	template<typename T>
	AssetPtr<T> Load(const wstring& _RelativePath, bool _IsEngineAsset = false);
	template<typename T>
	AssetPtr<T> LoadFromFile(const wstring& _Extention);
	/// <param name="_BindFlag">flags of D3D11_BIND_FLAG</param>
	AssetPtr<CTexture2D> CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
	AssetPtr<CTexture2D> CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D);
	// ���� ��ηκ��� Ȯ���ڸ� ���� ���� Ÿ���� ��ȯ�մϴ�.
	ASSET_TYPE GetAssetType(const wstring& _RelativePath);

	void ContentLoad();
	void ContentObserve();
	void ContentReload();

private:
	template<typename T>
	AssetPtr<T> FindAsset(const wstring& _RelativePath);
};


template<typename T>
ASSET_TYPE CAssetMgr::GetAssetType()
{
	if constexpr (is_same_v<CMesh, T>)
		return ASSET_TYPE::MESH;
	else if constexpr (is_same_v<CMaterial, T>)
		return ASSET_TYPE::MATERIAL;
	else if constexpr (is_same_v<CTexture2D, T>)
		return ASSET_TYPE::TEXTURE2D;
	else if constexpr (is_same_v<CSound, T>)
		return ASSET_TYPE::SOUND;
	else if constexpr (is_same_v<CGraphicShader, T>)
		return ASSET_TYPE::GRAPHIC_SHADER;
	else if constexpr (is_same_v<CComputeShader, T>)
		return ASSET_TYPE::COMPUTE_SHADER;
	else if constexpr (is_same_v<CPrefab, T>)
		return ASSET_TYPE::PREFAB;
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
inline AssetPtr<T> CAssetMgr::Load(const wstring& _RelativePath, bool _IsEngineAsset)
{
	if constexpr (is_base_of<CComputeShader, T>::value)
	{
		AssetPtr<CComputeShader> pAsset = FindAsset<CComputeShader>(_RelativePath);

		if (nullptr != pAsset)
			return dynamic_cast<T*>(pAsset.Get());

		pAsset = new T;
		pAsset->SetName(_RelativePath);
		pAsset->SetData();
		pAsset->Load(_RelativePath);
		CAssetMgr::GetInst()->AddAsset(_RelativePath, pAsset.Get());


		return dynamic_cast<T*>(pAsset.Get());
	}
	else
	{
		AssetPtr<T> pAsset = FindAsset<T>(_RelativePath);

		if (nullptr != pAsset)
			return pAsset;

		if (_IsEngineAsset)
			return nullptr;

		pAsset = new T;
		if (FAILED(pAsset->Load(_RelativePath)))
		{
			MessageBox(nullptr, L"���� �ε� ����", L"���� �ε� ����", MB_OK);
			return nullptr;
		}

		AddAsset(_RelativePath, pAsset.Get());


		return pAsset;
	}
}

// �ε� �� ������ Ȯ���ڸ� �Է��� ���� Ž����� ������ �ε��մϴ�. �ؽ���, ������ ��� �Է��� �����ϰ� �ڵ����� �ε��մϴ�.
template<typename T>
inline AssetPtr<T> CAssetMgr::LoadFromFile(const wstring& _Extention)
{
	// ���� ���� ����
	WCHAR filepath[255] = {};
	WCHAR filename[255] = {};
	wstring ContentDir = CPathMgr::GetContentDir();
	OPENFILENAME Desc = {};
	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = CEngine::GetInst()->GetMainWndHwnd();
	Desc.lpstrFilter = L"ALL\0*.*\0\0";
	Desc.lpstrFile = filepath;
	Desc.nMaxFile = 255;
	Desc.lpstrFileTitle = filename;
	Desc.nMaxFileTitle = 255;
	Desc.lpstrInitialDir = ContentDir.c_str();
	Desc.lpstrTitle = L"���Ϸκ��� ���� �ε�";
	Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (GetOpenFileName(&Desc))
	{
		std::wstring path = filepath;
		if (path.find(CPathMgr::GetContentDir()) == std::wstring::npos)
		{
			MessageBoxW(nullptr, L"���� ��ΰ� Content ���丮�� �ƴմϴ�.", L"Asset Load Error", MB_OK);
			return nullptr;
		}
		std::filesystem::path RelativePath = path.substr(ContentDir.size());
		std::filesystem::path EXT = RelativePath.extension();
		if (is_same_v<T, CTexture2D>)
		{
			if (EXT == L".dds" || EXT == L".DDS"
				|| EXT == L".tga" || EXT == L".TGA"
				|| EXT == L".png" || EXT == L".PNG"
				|| EXT == L".jpg" || EXT == L".JPG"
				|| EXT == L".jpeg" || EXT == L".JPEG"
				|| EXT == L".bmp" || EXT == L".BMP")
				return CAssetMgr::GetInst()->Load<T>(RelativePath);
		}
		else if (is_same_v<T, CSound>)
		{
			if (EXT == L".wav" || EXT == L".WAV"
				|| EXT == L".mp3" || EXT == L".MP3"
				|| EXT == L".aac" || EXT == L".AAC"
				|| EXT == L".ogg" || EXT == L".OGG"
				|| EXT == L".flac" || EXT == L".FLAC"
				|| EXT == L".wma" || EXT == L".WMA")
				return CAssetMgr::GetInst()->Load<T>(RelativePath);
		}
		else
		{
			if (EXT == _Extention)
				return CAssetMgr::GetInst()->Load<T>(RelativePath);
			else
				MessageBoxW(nullptr, L"�������� �ʴ� ���� �����Դϴ�.", L"Asset Load Error", MB_OK);
		}
	}
	return nullptr;
}

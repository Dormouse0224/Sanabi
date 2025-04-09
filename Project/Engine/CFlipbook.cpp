#include "pch.h"
#include "CFlipbook.h"

#include "CPathMgr.h"
#include "CAssetMgr.h"

CFlipbook::CFlipbook()
    : CAsset(ASSET_TYPE::FLIPBOOK)
{
}

CFlipbook::~CFlipbook()
{
}

int CFlipbook::GetSceneCount()
{
    if (m_SceneType == SceneType::SPRITE)
    {
        return m_vecSprite.size();
    }
    else if (m_SceneType == SceneType::TEXTURE)
    {
        return m_vecTex.size();
    }
}

int CFlipbook::Save(const wstring& _FileName)
{
    std::filesystem::path path = CPathMgr::GetContentDir() + std::wstring(L"Flipbook\\") + _FileName + std::wstring(L".flip");
    CPathMgr::CreateParentDir(path);
    std::fstream file(path, std::ios::out | std::ios::binary);

    // �ø��� Ÿ�� ����
    file.write(reinterpret_cast<char*>(&m_SceneType), sizeof(SceneType));

    if (m_SceneType == SceneType::SPRITE)
    {
        // ��������Ʈ�� Ű��(�̸�) ����
        int size = m_vecSprite.size();
        file.write(reinterpret_cast<char*>(&size), sizeof(int));
        for (int i = 0; i < size; ++i)
        {
            std::wstring SpriteName = {};
            if (m_vecSprite[i].Get())
                SpriteName = m_vecSprite[i]->GetName();
            int len = SpriteName.size();
            file.write(reinterpret_cast<char*>(&len), sizeof(int));
            file.write(reinterpret_cast<char*>(SpriteName.data()), sizeof(wchar_t) * len);
        }
    }
    else
    {
        // �ؽ����� Ű��(�̸�) ����
        int size = m_vecTex.size();
        file.write(reinterpret_cast<char*>(&size), sizeof(int));
        for (int i = 0; i < size; ++i)
        {
            std::wstring TexName = {};
            if (m_vecTex[i].Get())
                TexName = m_vecTex[i]->GetName();
            int len = TexName.size();
            file.write(reinterpret_cast<char*>(&len), sizeof(int));
            file.write(reinterpret_cast<char*>(TexName.data()), sizeof(wchar_t) * len);
        }
    }

    file.close();

    return S_OK;
}

int CFlipbook::Load(const wstring& _FilePath)
{
    std::filesystem::path path = CPathMgr::GetContentDir() + _FilePath;
    std::fstream file(path, std::ios::in | std::ios::binary);

    // �ø��� Ÿ�� �ҷ�����
    file.read(reinterpret_cast<char*>(&m_SceneType), sizeof(SceneType));

    if (m_SceneType == SceneType::SPRITE)
    {
        // ��������Ʈ�� Ű��(�̸�) �ҷ�����
        int size = 0;
        file.read(reinterpret_cast<char*>(&size), sizeof(int));
        for (int i = 0; i < size; ++i)
        {
            std::wstring SpriteName = {};
            int len = 0;
            file.read(reinterpret_cast<char*>(&len), sizeof(int));
            SpriteName.resize(len);
            file.read(reinterpret_cast<char*>(SpriteName.data()), sizeof(wchar_t) * len);

            // ��������Ʈ �ε�
            m_vecSprite[i] = CAssetMgr::GetInst()->Load<CSprite>(SpriteName);
            if (m_vecSprite[i] == nullptr)
            {
                file.close();
                MessageBoxW(nullptr, L"��������Ʈ �ε��� �����Ͽ����ϴ�.", L"Flipbook Load Error", MB_OK);
                return E_FAIL;
            }
        }
    }
    else
    {
        // �ؽ����� Ű��(�̸�) �ҷ�����
        int size = 0;
        file.read(reinterpret_cast<char*>(&size), sizeof(int));
        m_vecTex.resize(size);
        for (int i = 0; i < size; ++i)
        {
            std::wstring TexName = {};
            int len = 0;
            file.read(reinterpret_cast<char*>(&len), sizeof(int));
            TexName.resize(len);
            file.read(reinterpret_cast<char*>(TexName.data()), sizeof(wchar_t) * len);

            // �ؽ��� �ε�
            m_vecTex[i] = CAssetMgr::GetInst()->Load<CTexture2D>(TexName);
            if (m_vecTex[i] == nullptr)
            {
                file.close();
                MessageBoxW(nullptr, L"�ؽ��� �ε��� �����Ͽ����ϴ�.", L"Flipbook Load Error", MB_OK);
                return E_FAIL;
            }
        }
    }

    file.close();

    return S_OK;
}

AssetPtr<CFlipbook> CFlipbook::Create(const wstring& _Name)
{
    AssetPtr<CFlipbook> pNew = new CFlipbook;
    pNew->SetName(_Name);
    pNew->Save(_Name);
    return pNew;
}

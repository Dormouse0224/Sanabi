#include "pch.h"
#include "CSprite.h"

#include "CPathMgr.h"
#include "CAssetMgr.h"

CSprite::CSprite()
    : CAsset(ASSET_TYPE::SPRITE)
{
}

CSprite::~CSprite()
{
}

void CSprite::SetLeftTop(Vec2 _LeftTopPixel)
{
    if (nullptr == m_Atlas)
    {
        m_LeftTop = Vec2(0.f, 0.f);
        m_Slice = Vec2(0.f, 0.f);
        m_Background = Vec2(0.f, 0.f);
        m_Offset = Vec2(0.f, 0.f);
        return;
    }

    m_LeftTop = _LeftTopPixel / Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void CSprite::SetSlice(Vec2 _SlicePixel)
{
    if (nullptr == m_Atlas)
    {
        m_LeftTop = Vec2(0.f, 0.f);
        m_Slice = Vec2(0.f, 0.f);
        m_Background = Vec2(0.f, 0.f);
        m_Offset = Vec2(0.f, 0.f);
        return;
    }

    m_Slice = _SlicePixel / Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void CSprite::SetOffset(Vec2 _OffsetPixel)
{
    if (nullptr == m_Atlas)
    {
        m_LeftTop = Vec2(0.f, 0.f);
        m_Slice = Vec2(0.f, 0.f);
        m_Background = Vec2(0.f, 0.f);
        m_Offset = Vec2(0.f, 0.f);
        return;
    }
    m_Offset = _OffsetPixel / Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void CSprite::SetBackground(Vec2 _BackgroundPixel)
{
    if (nullptr == m_Atlas)
    {
        m_LeftTop = Vec2(0.f, 0.f);
        m_Slice = Vec2(0.f, 0.f);
        m_Background = Vec2(0.f, 0.f);
        m_Offset = Vec2(0.f, 0.f);
        return;
    }

    m_Background = _BackgroundPixel / Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

int CSprite::Save(const wstring& _FileName)
{
    std::filesystem::path path = CPathMgr::GetContentDir() + std::wstring(L"Sprite\\") + _FileName + std::wstring(L".sprite");
    CPathMgr::CreateParentDir(path);
    std::fstream file(path, std::ios::out | std::ios::binary);

    // ��Ʋ�� �ؽ����� Ű��(�̸�) ����
    std::wstring AtlasName = {};
    if (m_Atlas.Get())
        AtlasName = m_Atlas->GetName();
    int size = AtlasName.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(int));
    file.write(reinterpret_cast<char*>(AtlasName.data()), sizeof(wchar_t) * size);

    // ��������Ʈ �����̽� ���� ����
    file.write(reinterpret_cast<char*>(&m_LeftTop), sizeof(Vec2));
    file.write(reinterpret_cast<char*>(&m_Slice), sizeof(Vec2));
    file.write(reinterpret_cast<char*>(&m_Offset), sizeof(Vec2));
    file.write(reinterpret_cast<char*>(&m_Background), sizeof(Vec2));

    file.close();
    return S_OK;
}

int CSprite::Load(const wstring& _FilePath)
{
    std::filesystem::path path = CPathMgr::GetContentDir() + _FilePath;
    std::fstream file(path, std::ios::in | std::ios::binary);

    // ��Ʋ�� �ؽ����� Ű��(�̸�) �ҷ�����
    std::wstring AtlasName = {};
    int size = 0;
    file.read(reinterpret_cast<char*>(&size), sizeof(int));
    AtlasName.resize(size);
    file.read(reinterpret_cast<char*>(AtlasName.data()), sizeof(wchar_t) * size);

    // ��Ʋ�� �ؽ��� �ε�
    m_Atlas = CAssetMgr::GetInst()->Load<CTexture2D>(AtlasName);
    if (m_Atlas == nullptr)
    {
        file.close();
        MessageBoxW(nullptr, L"��Ʋ�� �ؽ��� �ε��� �����Ͽ����ϴ�.", L"Sprite Load Error", MB_OK);
        return E_FAIL;
    }

    // ��������Ʈ �����̽� ���� �ҷ�����
    file.read(reinterpret_cast<char*>(&m_LeftTop), sizeof(Vec2));
    file.read(reinterpret_cast<char*>(&m_Slice), sizeof(Vec2));
    file.read(reinterpret_cast<char*>(&m_Offset), sizeof(Vec2));
    file.read(reinterpret_cast<char*>(&m_Background), sizeof(Vec2));

    file.close();
    return S_OK;
}
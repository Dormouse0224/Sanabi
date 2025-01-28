#include "pch.h"
#include "CFlipbook.h"


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

int CFlipbook::Save(const wstring& _FilePath)
{
    return 0;
}

int CFlipbook::Load(const wstring& _FilePath)
{
    return 0;
}

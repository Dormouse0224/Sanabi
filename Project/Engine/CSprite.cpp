#include "pch.h"
#include "CSprite.h"

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

int CSprite::Save(const wstring& _FilePath)
{
    return 0;
}

int CSprite::Load(const wstring& _FilePath)
{
    return 0;
}
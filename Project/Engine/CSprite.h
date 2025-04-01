#pragma once
#include "CAsset.h"

#include "CTexture2D.h"

class CSprite :
    public CAsset
{
public:
    CSprite();
    ~CSprite();
    CLONE_DISABLE(CSprite);

private:
    AssetPtr<CTexture2D>   m_Atlas;
    Vec2            m_LeftTop;
    Vec2            m_Slice;
    Vec2            m_Offset;
    Vec2            m_Background;


public:
    void SetAtlasTex(AssetPtr<CTexture2D> _Tex) { m_Atlas = _Tex; }
    void SetLeftTop(Vec2 _LeftTopPixel);
    void SetSlice(Vec2 _SlicePixel);
    void SetOffset(Vec2 _OffsetPixel);
    void SetBackground(Vec2 _BackgroundPixel);

    AssetPtr<CTexture2D> GetAtlasTex() { return m_Atlas; }
    Vec2 GetLeftTop() { return m_LeftTop; }
    Vec2 GetSlice() { return m_Slice; }
    Vec2 GetOffset() { return m_Offset; }
    Vec2 GetBackground() { return m_Background; }

public:
    virtual int Save(const wstring& _FileName) override;
    virtual int Load(const wstring& _FilePath) override;

};


#pragma once
#include "CAsset.h"
class CFont :
    public CAsset
{
public:
    CFont();
    ~CFont();
    CLONE_DISABLE(CFont);

private:
    SpriteBatch* m_SpriteBatch;
    SpriteFont*  m_SpriteFont;

public:

    virtual int Save(const wstring& _FileName, bool _Update = false) { return S_OK; }
    virtual int Load(const wstring& _FilePath);

    void Render(wstring _Text, Vec2 _Pos, Vec4 _Color, float _Rot, Vec2 _Origin, float _Scale, float _Depth);
};
#pragma once
#include "CAsset.h"

#include "CSprite.h"

enum class SceneType
{
    SPRITE,
    TEXTURE,

};

class CFlipbook :
    public CAsset
{
public:
    CFlipbook();
    ~CFlipbook();
    CLONE_DISABLE(CFlipbook);

private:
    vector<AssetPtr<CSprite>>    m_vecSprite;
    vector<AssetPtr<CTexture2D>>   m_vecTex;

    SceneType               m_SceneType;

public:
    template<typename T>
    void AddScene(AssetPtr<T> _Scene)
    {
        if constexpr (is_same_v<CSprite, T>)
        {
            m_vecSprite.push_back(_Scene);
            m_SceneType = SceneType::SPRITE;
        }
        else if constexpr (is_same_v<CTexture2D, T>)
        {
            m_vecTex.push_back(_Scene);
            m_SceneType = SceneType::TEXTURE;
        }
    }

    int GetSceneCount();

    template<typename T>
    AssetPtr<T> GetScene(int _Idx)
    {
        if constexpr (is_same_v<CSprite, T>)
        {
            return m_vecSprite[_Idx];
        }
        else if constexpr (is_same_v<CTexture2D, T>)
        {
            return m_vecTex[_Idx];
        }
    }

    SceneType GetSceneType() { return m_SceneType; }

public:
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;

};


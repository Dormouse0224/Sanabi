#pragma once
#include "CRenderComponent.h"
#include "CFlipbookRenderUI.h"

#include "CFlipbook.h"

class CFlipbookRender :
    public CRenderComponent
{
    friend class CFlipbookRenderUI;
public:
    CFlipbookRender();
    ~CFlipbookRender();
    CLONE(CFlipbookRender);

private:
    vector<AssetPtr<CFlipbook>>     m_vecFlipbook;

    // 재생 정보
    AssetPtr<CFlipbook>             m_CurFlipbook;
    int                             m_SceneIdx;
    float                           m_Time;
    float                           m_FPS;
    bool                            m_Repeat;
    bool                            m_Finish;


public:
    void AddFlipbook(int _Idx, AssetPtr<CFlipbook> _Flipbook);
    void Play(int _Idx, float _FPS, bool _Repeat, int _SpriteIdx = 0)
    {
        m_CurFlipbook = m_vecFlipbook[_Idx];
        m_FPS = _FPS;
        m_Time = 0.f;
        m_Finish = false;
        m_Repeat = _Repeat;
        m_SceneIdx = 0;
    }


public:
    virtual void FinalTick() override;
    virtual void Render() override;

    virtual int Load(fstream& _Stream) override;
    virtual int Save(fstream& _Stream) override;

private:
    void CreateFlipbookMaterial();

};


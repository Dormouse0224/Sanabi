#pragma once
#include "CRenderComponent.h"

#include "CFlipbook.h"

class CFlipbookRender :
    public CRenderComponent
{
private:
    vector<Ptr<CFlipbook>>  m_vecFlipbook;
    Ptr<CFlipbook>          m_CurFlipbook;
    int                     m_SceneIdx;

    float                   m_Time;
    float                   m_FPS;
    bool                    m_Repeat;
    bool                    m_Finish;


public:
    void AddFlipbook(int _Idx, Ptr<CFlipbook> _Flipbook);
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

private:
    void CreateFlipbookMaterial();

public:
    CLONE(CFlipbookRender);
    CFlipbookRender();
    ~CFlipbookRender();
};


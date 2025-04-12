#pragma once
#include "CComponent.h"

// 이벤트 함수
typedef void (*UIEventFunc)(CGameObject* _Owner);

class CUICom :
    public CComponent
{
public:
    CUICom();
    ~CUICom();
    CLONE(CUICom)

private:
    UIEventFunc m_ClickEvent;
    UIEventFunc m_KeydownEvent;
    bool        m_MouseHover;
    bool        m_Clicked;

public:
    virtual void FinalTick();

    virtual int Save(fstream& _Stream);
    virtual int Load(fstream& _Stream);

    bool GetMouseHover() { return m_MouseHover; }
    bool GetClicked() { return m_Clicked; }

    void SetClickEvent(UIEventFunc _Func) { m_ClickEvent = _Func; }
    void SetKeydownEvent(UIEventFunc _Func) { m_KeydownEvent = _Func; }
};


#pragma once
#include "Engine\CScript.h"
class CCameraMoveScript :
    public CScript
{
public:
    CCameraMoveScript();
    ~CCameraMoveScript();
    CLONE(CCameraMoveScript);
private:
    float   m_CamSpeed;

public:
    virtual void Tick() override;

    virtual int Load(fstream& _Stream) override;
    virtual int Save(fstream& _Stream) override;

private:
    void Move_Perspective();
    void Move_OrthoGraphic();

protected:
    static CScript* Instantiate();

};


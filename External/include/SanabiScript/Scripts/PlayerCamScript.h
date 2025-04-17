#pragma once
#include "Engine\CScript.h"

class PlayerCamScript :
    public CScript
{
public:
    PlayerCamScript();
    PlayerCamScript(const PlayerCamScript& _Origin);
    ~PlayerCamScript();
    CLONE(PlayerCamScript);

public:
    virtual void Tick();
    virtual int Save(fstream& _Stream) { return S_OK; }
    virtual int Load(fstream& _Stream) { return S_OK; }
    virtual void Render_Script();
};


#pragma once
#include "CEntity.h"


class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer* m_Layer[MAX_LAYER];

public:
    void Begin();
    void Tick();
    void FinalTick();

public:
    CLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    void Deregister();


public:
    void AddGameObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove);

public:
    CLONE(CLevel);

public:
    CLevel();
    ~CLevel();
};


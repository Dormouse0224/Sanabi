#pragma once
#include "CEntity.h"


class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
public:
    CLevel();
    ~CLevel();
    CLONE(CLevel);

private:
    CLayer* m_Layer[(UINT)LAYER::END];

public:
    void Begin();
    void Tick();
    void FinalTick();

public:
    CLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    // 모든 레이어 모든 오브젝트를 클리어합니다.
    void Deregister();


public:
    void AddGameObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove);
};


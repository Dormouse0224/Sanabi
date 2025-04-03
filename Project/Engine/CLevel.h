#pragma once
#include "CEntity.h"

enum class LEVEL_STATE
{
    STOP,
    PLAY,
    PAUSE,

    NONE
};

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
    friend class CTaskMgr;
public:
    CLevel();
    CLevel(const CLevel& _Other);
    ~CLevel();
    CLONE(CLevel);

private:
    CLayer*     m_Layer[(UINT)LAYER::END];
    LEVEL_STATE m_State;

public:
    void Begin();
    void Tick();
    void FinalTick();

public:
    CLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    LEVEL_STATE GetState() { return m_State; }


    // 모든 레이어 모든 오브젝트를 클리어합니다.
    void Deregister();

private:
    void SetState(LEVEL_STATE _State);

public:
    // 오브젝트를 해당 레이어에 추가합니다.
    void AddGameObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove);
};


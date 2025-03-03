#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
    friend class CLevel;
public:
    CLayer();
    ~CLayer();
    CLONE(CLayer);

private:
    vector<CGameObject*>    m_vecParentObjects; // 레이어에 소속된 오브젝트 중, 최상위 부모 오브젝트만 관리
    vector<CGameObject*>    m_vecObjects;       // 최상위 부모, 자식 상관없이, 해당 레이어 소속 모든 오브젝트
    int                     m_LayerIdx;         // 레이어 번호

public:
    void Begin();
    void Tick();
    void FinalTick();

public:
    void AddGameObject(CGameObject* _Parent, bool _bChildMove);
    void RegisterGameObject(CGameObject* _Object) { m_vecObjects.push_back(_Object); }

    const vector<CGameObject*>& GetParentObjects() { return m_vecParentObjects; }
    const vector<CGameObject*>& GetObjects() { return m_vecObjects; }
};


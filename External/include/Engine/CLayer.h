#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
    //friend class CLevel;
public:
    CLayer();
    CLayer(const CLayer& _Other);
    ~CLayer();
    CLONE(CLayer);

private:
    vector<CGameObject*>    m_vecParentObjects; // ���̾ �Ҽӵ� ������Ʈ ��, �ֻ��� �θ� ������Ʈ�� ����
    // �ֻ��� �θ�, �ڽ� �������, �ش� ���̾� �Ҽ� ��� ������Ʈ.
    // �� ƽ���� Ŭ����ǰ�, ��� ������Ʈ�� FinalTick ������ ������Ʈ�� �ڽ��� ���̾ �����θ� RegisterGameObject �� �����.
    vector<CGameObject*>    m_vecObjects;
    int                     m_LayerIdx;         // ���̾� ��ȣ

public:
    void Begin();
    void End();
    void Tick();
    void Register();
    void FinalTick();

public:
    const vector<CGameObject*>& GetParentObjects() { return m_vecParentObjects; }
    const vector<CGameObject*>& GetObjects() { return m_vecObjects; }

    void SetLayerIdx(int _Idx) { m_LayerIdx = _Idx; }

    void AddGameObject(CGameObject* _Parent, bool _bChildMove);
    void RegisterGameObject(CGameObject* _Object) { m_vecObjects.push_back(_Object); }
    void UnRegisterParentObject(CGameObject* _Parent);
    void ClearVecObjects() { m_vecObjects.clear(); }

    int Save(fstream& _File, bool _Update = false);
    int Load(fstream& _File);

};


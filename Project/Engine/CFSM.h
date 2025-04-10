#pragma once
#include "CComponent.h"

class CFSM_State;

// State ��ȯ�� ������ �˻��ϴ� �Լ�. �� State �� ������� ���� ���������� ������ ó���Ͽ� State ��ȯ ���� ���� �� true �� ��ȯ�ϵ��� �Ѵ�.
typedef bool (*TriggerFunc)(CFSM_State* _Origin, CFSM_State* _Dest);

struct FSM_Condition
{
    FSM_Condition()
        : m_OriginState(nullptr)
        , m_DestState(nullptr)
        , m_TriggerFunc(nullptr)
    {
    }

    ~FSM_Condition()
    {
        delete m_OriginState;
        delete m_DestState;
    }

    CFSM_State* m_OriginState;
    CFSM_State* m_DestState;
    string      m_FuncName;
    TriggerFunc m_TriggerFunc;
};

class CFSM :
    public CComponent
{
    friend class CFSMUI;
public:
    CFSM();
    CFSM(const CFSM& _Other);
    ~CFSM();
    CLONE(CFSM);

private:
    vector<FSM_Condition*>              m_vecCondition;
    CFSM_State*                         m_CurrentState;
    CFSM_State*                         m_InitState;
    map<string, pair<CFSM_State*, int>> m_mapStates;    // FSM ������Ʈ�� ����ϴ� State ������ �����̳�. Condition ������ State ���� ���� ����Ѵ�.

public:
    virtual void FinalTick() override;
    virtual int Load(fstream& _Stream) override;
    virtual int Save(fstream& _Stream) override;

    void SetInitState(string _StateName);

    void AddCondition(const string& _Origin, const string& _Dest, const string& _FuncName);
    void DeleteCondition(int _Idx);

};

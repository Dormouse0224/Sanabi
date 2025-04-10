#pragma once
#include "CEntity.h"
class CFSM_State :
    public CEntity
{
public:
    CFSM_State();
    ~CFSM_State();
    CLONE_DISABLE(CFSM_State);

protected:
    // 상태 상수
    vector<int> m_vecInt;
    vector<float> m_vecFloat;
    vector<Vec2> m_vecVec2;
    vector<Vec4> m_vecVec4;


public:
    template<typename T>
    vector<T> GetConstVec();

    template<typename T>
    void SetConstVec(const vector<T>& _ConstVec);

    virtual void Tick() = 0;
    virtual void Begin() = 0;
    virtual void End() = 0;
};

template<typename T>
inline void CFSM_State::SetConstVec(const vector<T>& _ConstVec)
{
    if constexpr (std::is_same_v<T, int>)
    {
        m_vecInt = _ConstVec;
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        m_vecFloat = _ConstVec;
    }
    else if constexpr (std::is_same_v<T, Vec2>)
    {
        m_vecVec2 = _ConstVec;
    }
    else if constexpr (std::is_same_v<T, Vec4>)
    {
        m_vecVec4 = _ConstVec;
    }
}

template<typename T>
inline vector<T> CFSM_State::GetConstVec()
{
    if constexpr (std::is_same_v<T, int>)
    {
        return m_vecInt;
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        return m_vecFloat;
    }
    else if constexpr (std::is_same_v<T, Vec2>)
    {
        return m_vecVec2;
    }
    else if constexpr (std::is_same_v<T, Vec4>)
    {
        return m_vecVec4;
    }
}
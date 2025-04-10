#include "pch.h"
#include "CFSM.h"

#include "CFSMMgr.h"

#include "CFSM_State.h"

CFSM::CFSM()
	:CComponent(COMPONENT_TYPE::FSM)
	, m_vecCondition{}
	, m_CurrentState(nullptr)
{
}

CFSM::~CFSM()
{
	for (FSM_Condition* tCond : m_vecCondition)
		delete tCond;
}

void CFSM::FinalTick()
{
	// 등록된 조건 검사 실행
	for (FSM_Condition* tCond : m_vecCondition)
	{
		if (tCond->m_TriggerFunc(tCond->m_OriginState, tCond->m_DestState))
		{
			// 조건 만족 시 출발지/목적지 State 의 End/Begin 을 호출해 주고, 현재 State 를 갱신 후 반복문 즉시 종료
			tCond->m_OriginState->End();
			m_CurrentState = tCond->m_DestState;
			tCond->m_DestState->Begin();
			break;
		}
	}

}

int CFSM::Load(fstream& _Stream)
{
	// 조건 목록 불러오기
	int size = 0;
	_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
	for (int i = 0; i < size; ++i)
	{
		std::string ClassName = "";
		int size = 0;
		_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
		ClassName.resize(size);
		_Stream.read(reinterpret_cast<char*>(ClassName.data()), sizeof(char) * size);
		CFSM_State* pOrigin = CFSMMgr::GetInst()->CreateState(ClassName);

		ClassName = "";
		size = 0;
		_Stream.read(reinterpret_cast<char*>(&size), sizeof(int));
		ClassName.resize(size);
		_Stream.read(reinterpret_cast<char*>(ClassName.data()), sizeof(char) * size);
		CFSM_State* pDest = CFSMMgr::GetInst()->CreateState(ClassName);

		string FuncName = "";
		int len = 0;
		_Stream.read(reinterpret_cast<char*>(&len), sizeof(int));
		FuncName.resize(len);
		_Stream.read(reinterpret_cast<char*>(FuncName.data()), sizeof(char) * len);

		AddCondition(pOrigin, pDest, FuncName);
	}

	return S_OK;
}

int CFSM::Save(fstream& _Stream)
{
	// 조건 목록 저장
	int size = m_vecCondition.size();
	_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
	for (int i = 0; i < m_vecCondition.size(); ++i)
	{
		std::string ClassName = typeid(*(m_vecCondition[i]->m_OriginState)).name();
		int size = ClassName.size();
		_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
		_Stream.write(reinterpret_cast<char*>(ClassName.data()), sizeof(char) * size);

		ClassName = typeid(*(m_vecCondition[i]->m_DestState)).name();
		size = ClassName.size();
		_Stream.write(reinterpret_cast<char*>(&size), sizeof(int));
		_Stream.write(reinterpret_cast<char*>(ClassName.data()), sizeof(char) * size);

		int len = m_vecCondition[i]->m_FuncName.size();
		_Stream.write(reinterpret_cast<char*>(&len), sizeof(int));
		_Stream.write(reinterpret_cast<char*>(m_vecCondition[i]->m_FuncName.data()), sizeof(char) * len);
	}

	return S_OK;
}

void CFSM::AddCondition(CFSM_State* _Origin, CFSM_State* _Dest, const string& _FuncName)
{
	TriggerFunc func = CFSMMgr::GetInst()->GetTriggerFunc(_FuncName);
	if (func)
	{
		FSM_Condition* pCond = new FSM_Condition;
		pCond->m_OriginState = _Origin;
		pCond->m_DestState = _Dest;
		pCond->m_FuncName = _FuncName;
		pCond->m_TriggerFunc = func;

		auto iter = m_mapStates.find(typeid(*_Origin).name());
		if (iter != m_mapStates.end())
		{
			++(iter->second.second);
		}
		else
		{
			m_mapStates.insert(make_pair(typeid(*_Origin).name(), make_pair(_Origin, 1)));
		}

		iter = m_mapStates.find(typeid(*_Dest).name());
		if (iter != m_mapStates.end())
		{
			++(iter->second.second);
		}
		else
		{
			m_mapStates.insert(make_pair(typeid(*_Dest).name(), make_pair(_Dest, 1)));
		}
	}
}

void CFSM::DeleteCondition(int _Idx)
{
	if (_Idx < m_vecCondition.size())
	{
		auto iter = m_mapStates.find(typeid(*(m_vecCondition[_Idx]->m_OriginState)).name());
		--(iter->second.second);
		if (iter->second.second == 0)
		{
			m_mapStates.erase(iter);
		}

		iter = m_mapStates.find(typeid(*(m_vecCondition[_Idx]->m_DestState)).name());
		--(iter->second.second);
		if (iter->second.second == 0)
		{
			m_mapStates.erase(iter);
		}

		m_vecCondition.erase(m_vecCondition.begin() + _Idx);
	}
}

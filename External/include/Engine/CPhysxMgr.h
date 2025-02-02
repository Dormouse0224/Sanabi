#pragma once

class CGameObject;
class CGraphicShader;

//struct tActorData
//{
//	UINT		m_ID;
//	wstring		m_Name;
//};

class CPhysxMgr
	: public Singleton<CPhysxMgr>
{
public:
	SINGLE(CPhysxMgr);

private:
	// 초기화 데이터
	PxDefaultAllocator			m_DefaultAllocatorCallback;
	PxDefaultErrorCallback		m_DefaultErrorCallback;
	PxDefaultCpuDispatcher*		m_Dispatcher;
	PxTolerancesScale			m_ToleranceScale;

	PxFoundation*				m_Foundation;
	PxPhysics*					m_Physics;

	PxScene*					m_Scene;

	// 시뮬레이션 대상 관리 컨테이너
	map<CGameObject*, PxActor*>	m_mapActors;

	Ptr<CGraphicShader>			m_Shader;

public:
	void Init();
	void Tick();
	void Render();

	void AddDynamicActor(CGameObject* _Object, PxVec3 _Scale = PxVec3(1.f), PxVec3 _Offset = PxVec3(0.f));
	void AddStaticActor(CGameObject* _Object, PxVec3 _Scale = PxVec3(1.f), PxVec3 _Offset = PxVec3(0.f));
	PxActor* FindActor(CGameObject* _Object);

};


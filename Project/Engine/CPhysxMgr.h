#pragma once

class CGameObject;
class CGraphicShader;
class CSimulationEvent;

//struct tActorData
//{
//	UINT		m_ID;
//	wstring		m_Name;
//};

class CPhysxMgr
	: public Singleton<CPhysxMgr>
{
	//friend class CPhysxActor;
public:
	SINGLE(CPhysxMgr);

private:
	// �ʱ�ȭ ������
	PxDefaultAllocator					m_DefaultAllocatorCallback;
	PxDefaultErrorCallback				m_DefaultErrorCallback;
	PxDefaultCpuDispatcher*				m_Dispatcher;
	PxTolerancesScale					m_ToleranceScale;

	PxFoundation*						m_Foundation;
	PxPhysics*							m_Physics;

	PxScene*							m_Scene;

	// �ùķ��̼� ��� ���� �����̳�
	map<CGameObject*, PxRigidActor*>	m_mapRigidBody;

	// ����׿� ���̴�
	AssetPtr<CGraphicShader>			m_Shader;

	// �浹 �̺�Ʈ ó�� ��ü
	CSimulationEvent*					m_EventCallback;

public:
	void Init();
	void Tick();
	void Render();

	PxPhysics* GetPhysics() { return m_Physics; }

	PxRigidActor* FindRigidBody(CGameObject* _Object);
	void AddRigidBody(CGameObject* _Object, PxRigidActor* _Actor);
	void RemoveRigidBody(CGameObject* _Object);
};


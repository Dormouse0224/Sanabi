#pragma once
#include "CComponent.h"
#include "CPhysxMgr.h"

// �浹 �̺�Ʈ �߻� �� ȣ��Ǵ� �Լ�
typedef void (*ContactFunc)(CGameObject*);

struct COLLIDER_DESC
{
    PxShapeFlag::Enum   ShapeFlag;
    PxReal              StaticFriction      = 0.f;
    PxReal              DynamicFriction     = 0.f;
    PxReal              Restitution         = 0.f;
    PxU32               FilterLayer_Self;
    PxU32               FilterLayer_Other;
};

enum class RIGID_TYPE
{
    KINEMATIC,
    DYNAMIC,
    STATIC,

    NONE,
};

enum LOCK_FLAG
{
    NONE        = 0,
    LINEAR_X    = (1<<0),
    LINEAR_Y    = (1<<1),
    LINEAR_Z    = (1<<2),
    ANGULAR_X   = (1<<3),
    ANGULAR_Y   = (1<<4),
    ANGULAR_Z   = (1<<5),

    LINEAR_ALL  = LINEAR_X | LINEAR_Y | LINEAR_Z,
    ANGULAR_ALL = ANGULAR_X | ANGULAR_Y | ANGULAR_Z,
};

extern wstring LOCK_FLAG_WSTR[];

class CPhysxActor :
    public CComponent
{
public:
    CPhysxActor();
    CPhysxActor(const CPhysxActor& _Other);
    ~CPhysxActor();
    CLONE(CPhysxActor);

private:
    PxRigidActor*           m_Body;
    RIGID_TYPE              m_Type;
    UINT                    m_LockFlag;
    bool                    m_Gravity;
    float                   m_Density;

    vector<COLLIDER_DESC>   m_vecDesc;
    vector<PxVec3>          m_vecScale;
    vector<PxVec3>          m_vecOffset;

    // ���� ���� �� Body �� Collider ������ �Ұ���(Owner Object �� �������� ���� Ÿ�̹��̱� ����)�ϹǷ�, ���� FinalTick ���� ���� �ʱ�ȭ�� �����Ѵ�.
    // ���� ���� ���� �ÿ��� true �� �ԷµǸ�, FinalTick ���� �߰� �ʱ�ȭ ���� �� false �� �ȴ�.
    bool                    m_DelayedInit;

    ContactFunc             m_ContactBegin;
    ContactFunc             m_ContactTick;
    ContactFunc             m_ContactEnd;

public:
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void FinalTick();

public:
    PxRigidActor* GetRigidBody() { return m_Body; }
    RIGID_TYPE GetRigidType() { return m_Type; }
    bool GetGravity() { return m_Gravity; }
    float GetDensity() { return m_Density; }

    void SetRigidBody(RIGID_TYPE _Type, UINT _LockFlag = 0, bool _Gravity = false, float _Density = 1.f, PxVec3 _InitVel = PxVec3(0.f));
    void SetRigidType(RIGID_TYPE _Type);
    void SetGravity(bool _Gravity);
    void SetDensity(float _Density);

    void AddCollider(COLLIDER_DESC _desc, PxVec3 _Scale = PxVec3(1.f), PxVec3 _Offset = PxVec3(0.f));
    void UpdatePosition(Vec3 _Pos);
    void UpdateRotation(Vec4 _RotQuat);
    void CkeckLockFlag(LOCK_FLAG _Flag);

    void SetContactBegin(ContactFunc _Func) { m_ContactBegin = _Func; }
    void SetContactTick(ContactFunc _Func) { m_ContactTick = _Func; }
    void SetContactEnd(ContactFunc _Func) { m_ContactEnd = _Func; }

    void ContactBegin(CGameObject* _Other) { if (m_ContactBegin) { m_ContactBegin(_Other); } }
    void ContactTick(CGameObject* _Other) { if (m_ContactTick) { m_ContactTick(_Other); } }
    void ContactEnd(CGameObject* _Other) { if (m_ContactEnd) { m_ContactEnd(_Other); } }

    virtual int Load(fstream& _Stream) override;
    virtual int Save(fstream& _Stream) override;

private:
    void AttachCollider(COLLIDER_DESC _desc, PxVec3 _Scale, PxVec3 _Offset);
};


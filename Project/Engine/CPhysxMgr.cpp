#include "pch.h"

#include "CPhysxMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CGameObject.h"
#include "CDevice.h"
#include "CGraphicShader.h"
#include "CConstBuffer.h"

CPhysxMgr::CPhysxMgr()
    : m_Dispatcher(nullptr)
    , m_Foundation(nullptr)
    , m_Physics(nullptr)
    , m_Scene(nullptr)
{
}

CPhysxMgr::~CPhysxMgr()
{
    m_Scene->release();
    m_Physics->release();
    m_Dispatcher->release();
    m_Foundation->release();
}

void CPhysxMgr::Init()
{
    // physx �ʱ�ȭ
    m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
    if (!m_Foundation) throw("PxCreateFoundation failed!");
    m_ToleranceScale.length = 1.f;
    m_ToleranceScale.speed = 0.1f;
    m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale);

    PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.f, -200.f, 0.f);
    m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_Dispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    m_Scene = m_Physics->createScene(sceneDesc);

    m_Scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    m_Scene->setVisualizationParameter(PxVisualizationParameter::eBODY_AXES, 10.0f);
    m_Scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);


    // physx ����׿� ���̴� ������
    m_Shader = new CGraphicShader;

    m_Shader->CreateVertexShader(L"HLSL\\PhysxDebug.fx", "VS_PhysxDebug");
    m_Shader->CreatePixelShader(L"HLSL\\PhysxDebug.fx", "PS_PhysxDebug");
    m_Shader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);
    m_Shader->SetRSType(RS_TYPE::CULL_NONE);
    m_Shader->SetDSType(DS_TYPE::LESS);
    m_Shader->SetBSType(BS_TYPE::ALPHABLEND);
    m_Shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void CPhysxMgr::Tick()
{
    // ���� �ùķ��̼� ����
    m_Scene->simulate(DT);
    m_Scene->fetchResults(true);

    // �ùķ��̼� �� ��� ���� ��ü ���� ��ȯ, ��ǥ ������Ʈ
    vector<PxActor*> vecDynamicActor(m_mapActors.size(), nullptr);
    m_Scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, vecDynamicActor.data(), vecDynamicActor.size());

    for (int i = 0; i < vecDynamicActor.size(); ++i)
    {
        CGameObject* pObject = (CGameObject*)(vecDynamicActor[i]->userData);
        PxVec3 Trans = ((PxRigidDynamic*)(vecDynamicActor[i]))->getGlobalPose().p;
        pObject->Transform()->SetRelativePos(Trans.x, Trans.y, Trans.z);
    }
}

void CPhysxMgr::Render()
{
    // ����� ������ ������ ���� ������ ġȯ ��, ���� ���۸� ����
    const PxRenderBuffer& RenderBuffer = m_Scene->getRenderBuffer();

    ComPtr<ID3D11Buffer> VB = nullptr;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = RenderBuffer.getNbLines() * sizeof(Vtx) * 2;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.CPUAccessFlags = 0;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    vector<Vtx> vecVtx = {};
    for (PxU32 i = 0; i < RenderBuffer.getNbLines(); i++)
    {
        const PxDebugLine& line = RenderBuffer.getLines()[i];
        Vtx vtx1, vtx2;
        vtx1.vPos = Vec3(line.pos0.x, line.pos0.y, line.pos0.z);
        vtx2.vPos = Vec3(line.pos1.x, line.pos1.y, line.pos1.z);
        vtx1.vColor = Vec4(((line.color0 >> 16) & 0xFF) / 255.f, ((line.color0 >> 8) & 0xFF) / 255.f, ((line.color0 >> 0) & 0xFF) / 255.f, ((line.color0 >> 24) & 0xFF) / 255.f);
        vtx2.vColor = Vec4(((line.color1 >> 16) & 0xFF) / 255.f, ((line.color1 >> 8) & 0xFF) / 255.f, ((line.color1 >> 0) & 0xFF) / 255.f, ((line.color1 >> 24) & 0xFF) / 255.f);
        vecVtx.push_back(vtx1);
        vecVtx.push_back(vtx2);
    }

    D3D11_SUBRESOURCE_DATA sub = {};
    sub.pSysMem = vecVtx.data();

    DEVICE->CreateBuffer(&desc, &sub, VB.GetAddressOf());

    // ���̴� �� ���� ���� ���ε�
    m_Shader->Binding();

    // ������Ʈ�� ��ġ�� ������۷� �̵���Ų��.
    CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

    g_Trans.matWorld = Matrix::Identity;
    g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
    g_Trans.matVP = g_Trans.matView * g_Trans.matProj;
    g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;

    pTransformBuffer->SetData(&g_Trans, sizeof(tTransform));
    pTransformBuffer->Binding();

    UINT stride = sizeof(Vtx);
    UINT Offset = 0;
    CONTEXT->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &Offset);

    // ����
    CONTEXT->Draw(vecVtx.size(), 0);
}

void CPhysxMgr::AddDynamicActor(CGameObject* _Object, PxVec3 _Scale, PxVec3 _Offset)
{
    // �������, ź�����, ��� ���� �� ��ǥ ��������
    PxMaterial* pMaterial = m_Physics->createMaterial(0.f, 0.f, 0.f);
    pMaterial->setRestitutionCombineMode(PxCombineMode::eMIN);
    PxShape* shape = m_Physics->createShape(PxBoxGeometry(_Scale), *pMaterial);
    shape->setLocalPose(PxTransform(_Offset));
    PxTransform Trans(PxVec3(_Object->Transform()->GetRelativePos().x, _Object->Transform()->GetRelativePos().y, _Object->Transform()->GetRelativePos().z));

    // ��ü ����, ���� ���� ���
    PxRigidDynamic* pBody = m_Physics->createRigidDynamic(Trans);
    pBody->attachShape(*shape);
    pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);       // Z�� �̵� ���
    pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);      // X�� ȸ�� ���
    pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);      // Y�� ȸ�� ���
    pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);      // Z�� ȸ�� ���
    pBody->userData = _Object;
    PxRigidBodyExt::setMassAndUpdateInertia(*pBody, 100.f);
    //PxRigidBodyExt::updateMassAndInertia(*pBody, 0.1f);
    m_Scene->addActor(*pBody);

    // ������Ʈ�� �ùķ��̼� ���� ��Ͽ� �߰�
    m_mapActors.insert(make_pair(_Object, pBody));

    shape->release();
}

void CPhysxMgr::AddStaticActor(CGameObject* _Object, PxVec3 _Scale, PxVec3 _Offset)
{
    // �������, ź�����, ��� ���� �� ��ǥ ��������
    PxMaterial* pMaterial = m_Physics->createMaterial(0.f, 0.f, 0.f);
    PxShape* shape = m_Physics->createShape(PxBoxGeometry(_Scale), *pMaterial);
    shape->setLocalPose(PxTransform(_Offset));
    PxTransform Trans(PxVec3(_Object->Transform()->GetRelativePos().x, _Object->Transform()->GetRelativePos().y, _Object->Transform()->GetRelativePos().z));

    PxRigidStatic* pBody = m_Physics->createRigidStatic(Trans);
    pBody->attachShape(*shape);
    pBody->userData = _Object;
    m_Scene->addActor(*pBody);

    // ������Ʈ�� �ùķ��̼� ���� ��Ͽ� �߰�
    //m_mapActors.insert(make_pair(_Object, pBody));

    shape->release();
}



PxActor* CPhysxMgr::FindActor(CGameObject* _Object)
{
    map<CGameObject*, PxActor*>::iterator iter = m_mapActors.find(_Object);

    if (iter == m_mapActors.end())
        return nullptr;

    return iter->second;
}

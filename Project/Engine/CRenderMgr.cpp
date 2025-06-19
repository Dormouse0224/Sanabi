#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLevel.h"
#include "CRenderComponent.h"
#include "CLayer.h"
#include "CMRT.h"

#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CPhysxMgr.h"
#include "CLevelMgr.h"
#include "CImguiMgr.h"
#include "CLightMgr.h"

CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_DebugObject)
		delete m_DebugObject;

	if (nullptr != m_EditorCam)
		delete m_EditorCam;

	if (nullptr != m_UICam)
		delete m_UICam;

	for (int i = 0; i < MRT_END; ++i)
	{
		if (m_arrMRT[i] != nullptr)
			delete m_arrMRT[i];
	}
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Priority)
{
	assert(_Priority >= 0);	// Priority �� 0 �̻��̾�� �մϴ�.

	if (m_vecCam.size() <= _Priority)
	{
		m_vecCam.resize(_Priority + 1);
	}

	// �Է¹��� priority �ڸ��� �ٸ� ī�޶� �̹� �ִ� ���, ���� ī�޶� priority -1 �� ���� �� �ڸ��� ���� ����.
	if (m_vecCam[_Priority] != nullptr)
	{
		m_vecCam[_Priority]->m_Priority = -1;
		m_vecCam[_Priority]->m_Registered = false;
	}

	m_vecCam[_Priority] = _Cam;
	m_vecCam[_Priority]->m_Registered = true;
}

void CRenderMgr::ResetEditorCamPos()
{
	m_EditorCam->Transform()->SetRelativePos(0, 0, 0);
	m_EditorCam->Transform()->SetRelativeRot(Vec3(0, 0, 0));
}


void CRenderMgr::Tick()
{
	// ���� ������ ���ε�
	CLightMgr::GetInst()->Binding();

	// Global ������ ���ε�
	static CConstBuffer* pGlobal = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobal->SetData(&g_global, sizeof(tGlobal));
	pGlobal->Binding();
	pGlobal->Binding_CS();

	m_EditorCam->Tick();
	m_EditorCam->FinalTick();

	m_UICam->Tick();
	if (m_CurrentCam)
	{
		m_UICam->Transform()->SetRelativePos(m_CurrentCam->GetOwner()->Transform()->GetRelativePos());
		m_UICam->Transform()->SetRelativeRot(m_CurrentCam->GetOwner()->Transform()->GetRelativeRotQuat());
	}
	m_UICam->FinalTick();

	// ���� Ÿ�� Ŭ����
	ClearMRT();

	// Main ������	
	Render();

	// ���� ����� ������
	MergeRender();
}

void CRenderMgr::Render()
{
	// ��� ����Ÿ�� �� ��� ����Ÿ�� ����
	m_arrMRT[MRT_TYPE::DEFERRED]->SetRenderTarget();

	// ���� ������ ���ų� Stop �̸� ������ ī�޶� ���, �ƴϸ� ������ ī�޶� ������Ʈ�� ���.
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel == nullptr || pLevel->GetState() == LEVEL_STATE::STOP)
	{
		m_CurrentCam = m_EditorCam->Camera();
		m_EditorCam->Camera()->Render();
	}
	else
	{
		for (size_t i = 0; i < m_vecCam.size(); ++i)
		{
			m_CurrentCam = m_vecCam[i];
			m_vecCam[i]->Render();
		}
	}
}

void CRenderMgr::UIRender()
{
	m_UICam->Camera()->Render();
}

void CRenderMgr::DebugRender()
{
	vector<tDebugShapeInfo>::iterator iter = m_vecDebugInfo.begin();
	for (; iter != m_vecDebugInfo.end(); )
	{
		// ����� ��翡 ���� �޽� ����
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->GetRenderComponent()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"RectMesh_Debug", true));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->GetRenderComponent()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"CircleMesh_Debug", true));
			break;
		case DEBUG_SHAPE::LINE:
			break;
		case DEBUG_SHAPE::CUBE:
			break;
		case DEBUG_SHAPE::SPHERE:
			break;
		default:
			break;
		}

		// ���� ������ ���� DepthStencilState �� �����		
		if ((*iter).DepthTest)
			m_DebugObject->GetRenderComponent()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_DebugObject->GetRenderComponent()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WIRITE);

		// ������ ������ ���ؼ� ����
		m_DebugObject->GetRenderComponent()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).Color);

		// ��ġ���� ���� �� ������� ���
		if ((*iter).MatWorld == Matrix::Identity)
		{
			m_DebugObject->Transform()->SetRelativePos((*iter).WorldPos);
			m_DebugObject->Transform()->SetRelativeScale((*iter).WorldScale);
			m_DebugObject->Transform()->SetRelativeRot((*iter).WorldRotation);
			m_DebugObject->Transform()->FinalTick();
		}
		else
		{
			m_DebugObject->Transform()->SetWorldMat((*iter).MatWorld);
		}

		// ����� ������Ʈ ������
		m_DebugObject->Render();

		// ���� �ð��� �Ѿ ������ �����Ѵ�.
		(*iter).CurTime += EngineDT;
		if ((*iter).Duration < (*iter).CurTime)
			iter = m_vecDebugInfo.erase(iter);
		else
			++iter;
	}
}

void CRenderMgr::MergeRender()
{
	// ��� ����Ÿ�� �� ��� ����Ÿ�� ����
	m_arrMRT[MRT_TYPE::MERGE]->SetRenderTarget();

	m_MergeMtrl->SetTexParam(TEX_0, m_arrMRT[MRT_TYPE::DEFERRED]->GetRenderTarget(0));
	//m_BackBufferMtrl->SetTexParam(TEX_0, CDevice::GetInst()->GetLightRTT());

	m_MergeMtrl->Binding();

	m_MergeMesh->Render();

	// Debug ������
	DebugRender();

	// Physx ������
	CPhysxMgr::GetInst()->Render();

	// ImGui ������
	CImguiMgr::GetInst()->Render();

	// Ŀ�� ������
	CKeyMgr::GetInst()->Render();
}

void CRenderMgr::ClearMRT()
{
	for (int i = 0; i < MRT_END; ++i)
	{
		if (m_arrMRT[i])
			m_arrMRT[i]->ClearTargets();
	}
}
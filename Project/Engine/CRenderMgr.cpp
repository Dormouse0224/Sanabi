#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLevel.h"
#include "CRenderComponent.h"
#include "CLayer.h"


#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CPhysxMgr.h"
#include "CLevelMgr.h"

CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_DebugObject)
		delete m_DebugObject;
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, UINT _Priority)
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


void CRenderMgr::Tick()
{
	CDevice::GetInst()->SetRenderTargetAndViewport();
	//// ��� ����Ÿ�� �� ��� ����Ÿ�� ����
	//AssetPtr<CTexture2D> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture2D>(L"RenderTarget");
	//AssetPtr<CTexture2D> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture2D>(L"DepthStencil");
	//CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());


	// GlobalData Binding
	static CConstBuffer* pGlobal = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobal->SetData(&g_global, sizeof(tGlobal));
	pGlobal->Binding();
	pGlobal->Binding_CS();

	// Main Rendering	
	Render();

	// Debug Rendering
	DebugRender();

	// Physx Rendering
	CPhysxMgr::GetInst()->Render();
}

void CRenderMgr::Render()
{
	CDevice::GetInst()->ClearTarget();

	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->Render();
	}
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
			m_DebugObject->GetRenderComponent()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->GetRenderComponent()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
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
			m_DebugObject->Transform()->SetRelativeRotation((*iter).WorldRotation);
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
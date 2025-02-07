#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CCamera.h"
#include "CTransform.h"

#include "CAssetMgr.h"
#include "CRenderComponent.h"

#include "CTimeMgr.h"
#include "CPhysxMgr.h"

CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_DebugObject)
		delete m_DebugObject;
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Priority)
{
	if (m_vecCam.size() <= _Priority)
	{
		m_vecCam.resize(_Priority + 1);
	}

	if (m_vecCam[_Priority] != nullptr)
	{
		m_vecCam[_Priority]->m_Priority = -1;
	}

	m_vecCam[_Priority] = _Cam;
}


void CRenderMgr::Tick()
{
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

	// 윈도우 화면에 송출
	CDevice::GetInst()->Present();
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
		// 디버그 모양에 따른 메쉬 선택
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

		// 깊이 판정에 따라서 DepthStencilState 를 골라줌		
		if ((*iter).DepthTest)
			m_DebugObject->GetRenderComponent()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_DebugObject->GetRenderComponent()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WIRITE);

		// 색상값을 재질을 통해서 전달
		m_DebugObject->GetRenderComponent()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).Color);

		// 위치정보 세팅 및 월드행렬 계산
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

		// 디버그 오브젝트 렌더링
		m_DebugObject->Render();

		// 노출 시간이 넘어선 정보는 제거한다.
		(*iter).CurTime += DT;
		if ((*iter).Duration < (*iter).CurTime)
			iter = m_vecDebugInfo.erase(iter);
		else
			++iter;
	}
}
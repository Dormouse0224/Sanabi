#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CTransform.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}


void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{
	// ��ġ���� ������Ʈ
	Transform()->Binding();

	// ����� ���̴� ���ε�
	GetMaterial()->Binding();

	// ������ ����
	GetMesh()->Render();
}
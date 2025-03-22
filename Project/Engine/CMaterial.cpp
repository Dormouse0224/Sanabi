#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTexture2D.h"

CMaterial::CMaterial()
	: CAsset(ASSET_TYPE::MATERIAL)
	, m_Const{}
{

}

CMaterial::~CMaterial()
{
}

void CMaterial::Binding()
{
	m_Shader->Binding();

	for (UINT i = 0; i < TEX_PARAM::TEX_END; ++i)
	{
		if (nullptr != m_arrTex[i].Get())
		{
			m_arrTex[i]->Binding(i);
		}
	}

	// ���� ������� ������ۿ� ���� �� ���ε�
	static CConstBuffer* pBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pBuffer->SetData(&m_Const, sizeof(tMtrlConst));
	pBuffer->Binding();
}

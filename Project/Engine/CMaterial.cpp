#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTexture2D.h"

#include "CPathMgr.h"
#include "CAssetMgr.h"

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

int CMaterial::Save(const wstring& _FileName)
{
	std::filesystem::path path = CPathMgr::GetContentDir() + std::wstring(L"Material\\") + _FileName + std::wstring(L".mtrl");
	if (!std::filesystem::exists(path.parent_path()))
		std::filesystem::create_directories(path.parent_path());
	std::fstream file(path, std::ios::out | std::ios::binary);

	// ������ ����ϴ� �׷��� ���̴��� Ű��(�̸�) ����
	std::wstring ShaderName = m_Shader->GetName();
	int size = ShaderName.size();
	file.write(reinterpret_cast<char*>(&size), sizeof(int));
	file.write(reinterpret_cast<char*>(ShaderName.data()), sizeof(wchar_t) * size);

	// ������ ����ϴ� �ؽ����� Ű��(�̸�) ����
	for (int i = 0; i < TEX_END; ++i)
	{
		std::wstring TexName = {};
		if (m_arrTex[i].Get())
			TexName = m_arrTex[i]->GetName();
		int size = TexName.size();
		file.write(reinterpret_cast<char*>(&size), sizeof(int));
		file.write(reinterpret_cast<char*>(TexName.data()), sizeof(wchar_t) * size);
	}

	// ���� ��� ����ü ����
	file.write(reinterpret_cast<char*>(&m_Const), sizeof(tMtrlConst));

	file.close();
	return S_OK;
}

int CMaterial::Load(const wstring& _FilePath)
{
	std::filesystem::path path = CPathMgr::GetContentDir() + _FilePath;
	std::fstream file(path, std::ios::in | std::ios::binary);

	// ������ ����ϴ� �׷��� ���̴��� Ű��(�̸�) �ҷ�����
	std::wstring ShaderName = {};
	int size = 0;
	file.read(reinterpret_cast<char*>(&size), sizeof(int));
	ShaderName.resize(size);
	file.read(reinterpret_cast<char*>(ShaderName.data()), sizeof(wchar_t) * size);

	// �׷��� ���̴� �ε�
	m_Shader = CAssetMgr::GetInst()->Load<CGraphicShader>(ShaderName);
	if (m_Shader == nullptr)
	{
		file.close();
		MessageBoxW(nullptr, L"�׷��� ���̴� �ε��� �����߽��ϴ�.", L"Material Load Error", MB_OK);
		return E_FAIL;
	}

	// ������ ����ϴ� �ؽ����� Ű��(�̸�) �ҷ�����
	for (int i = 0; i < TEX_END; ++i)
	{
		std::wstring TexName = {};
		int size = TexName.size();
		file.read(reinterpret_cast<char*>(&size), sizeof(int));
		TexName.resize(size);
		file.read(reinterpret_cast<char*>(TexName.data()), sizeof(wchar_t) * size);

		// �ؽ��� �ε�
		if (!TexName.empty())
		{
			m_arrTex[i] = CAssetMgr::GetInst()->Load<CTexture2D>(TexName);
			if (m_arrTex[i] == nullptr)
			{
				file.close();
				MessageBoxW(nullptr, L"�ؽ��� �ε��� �����߽��ϴ�.", L"Material Load Error", MB_OK);
				return E_FAIL;
			}
		}
	}

	// ���� ��� ����ü �ҷ�����
	file.read(reinterpret_cast<char*>(&m_Const), sizeof(tMtrlConst));

	file.close();

	return S_OK;
}

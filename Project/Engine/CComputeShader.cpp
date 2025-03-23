#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CPathMgr.h"

CComputeShader::CComputeShader()
	: CShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_ThreadPerGroupX(1)
	, m_ThreadPerGroupY(1)
	, m_ThreadPerGroupZ(1)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
	, m_CSTex{}
{
}

CComputeShader::CComputeShader(const wstring& _RelativePath, const string& _FuncName
	, UINT _GroupPerX, UINT _GroupPerY, UINT _GroupPerZ)
	: CShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_ThreadPerGroupX(_GroupPerX)
	, m_ThreadPerGroupY(_GroupPerY)
	, m_ThreadPerGroupZ(_GroupPerZ)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
	, m_CSTex{}
{
	CreateComputeShader(_RelativePath, _FuncName);
}

CComputeShader::~CComputeShader()
{
}

int CComputeShader::CreateComputeShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetContentPath() + _RelativePath;

	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "cs_5_0", Flag, 0
		, m_CSBlob.GetAddressOf()
		, m_ErrBlob.GetAddressOf())))
	{
		// ������ �����ϸ� �޼��� �ڽ��� ������ ���г��� ����
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "���̴� ������ ����", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "���̴� ���� ��ΰ� �Źٸ��� �ʽ��ϴ�.", "���̴� ������ ����", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer()
		, m_CSBlob->GetBufferSize()
		, nullptr
		, m_CS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CComputeShader::Execute()
{
	// ���ҽ� �� ���̴� ���ε�
	if (FAILED(Binding()))
	{
		assert(nullptr);
		return E_FAIL;
	}

	CalcGroupNum();

	// ������� ������ۿ� ���� �� CS ���ε�
	static CConstBuffer* pBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pBuffer->SetData(&m_Const, sizeof(tMtrlConst));
	pBuffer->Binding_CS();

	// �ؽ�ó ���ε�
	for (int i = 0; i < TEX_PARAM::TEX_END; ++i)
	{
		if (m_CSTex[i].first != nullptr)
			m_CSTex[i].first->Binding_CS_SRV(m_CSTex[i].second);
	}

	// ��ǻƮ ���̴� ���ε� �� ����
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	// ���ҽ� �� ���̴� ���ε� Ŭ����
	Clear();
	for (int i = 0; i < TEX_PARAM::TEX_END; ++i)
	{
		if (m_CSTex[i].first != nullptr)
			m_CSTex[i].first->Clear_CS_SRV(m_CSTex[i].second);
	}
	CONTEXT->CSSetShader(nullptr, nullptr, 0);
}

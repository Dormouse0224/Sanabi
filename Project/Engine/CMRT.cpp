#include "pch.h"
#include "CMRT.h"

#include "CAssetMgr.h"
#include "CDevice.h"
#include "CTexture2D.h"

CMRT::CMRT()
	: m_RTTex{}
	, m_DSTex(nullptr)
	, m_TexCount(0)
	, m_DepthClear(1.f)
	, m_StencilClear(0)
{
}

CMRT::CMRT(const CMRT& _Other)
{
}

CMRT::~CMRT()
{
}

int CMRT::CreateRenderTarget(D3D11_TEXTURE2D_DESC _Desc, wstring _Key, Vec4 _ClearColor)
{
	// ���� Ÿ���� �̹� 8�� �̰ų� �ش� Ű������ �̹� ���ҽ��� �����ϴ� ���, �Ǵ� ���ε� �÷��װ� ���� Ÿ���� �ƴ� ��� ���� ����
	if (m_TexCount == 8 || CAssetMgr::GetInst()->Load<CTexture2D>(_Key, true) != nullptr || !(_Desc.BindFlags & D3D11_BIND_RENDER_TARGET))
		return E_FAIL;

	ComPtr<ID3D11Texture2D> pRTTex = nullptr;
	if (FAILED(DEVICE->CreateTexture2D(&_Desc, nullptr, pRTTex.GetAddressOf())))
	{
		return E_FAIL;
	}

	m_RTTex[m_TexCount] = CAssetMgr::GetInst()->CreateTexture(_Key, pRTTex);
	m_ClearColor[m_TexCount] = _ClearColor;
	++m_TexCount;
	return S_OK;
}

int CMRT::CreateRenderTarget(ComPtr<ID3D11Texture2D> _RTTex, wstring _Key, Vec4 _ClearColor)
{
	D3D11_TEXTURE2D_DESC Desc;
	_RTTex->GetDesc(&Desc);

	// ���� Ÿ���� �̹� 8�� �̰ų� ���ε� �÷��װ� ���� Ÿ���� �ƴ� ��� ���� ����
	if (m_TexCount == 8 || !(Desc.BindFlags & D3D11_BIND_RENDER_TARGET))
		return E_FAIL;

	m_RTTex[m_TexCount] = CAssetMgr::GetInst()->CreateTexture(_Key, _RTTex);
	m_ClearColor[m_TexCount] = _ClearColor;
	++m_TexCount;
	return S_OK;
}

int CMRT::CreateDepthStencil(D3D11_TEXTURE2D_DESC _Desc, wstring _Key, float _DepthClear, UINT8 _StencilClear)
{
	// �ش� Ű������ �̹� ���ҽ��� �����ϴ� ���, �Ǵ� ���ε� �÷��װ� ���� ���ٽ��� �ƴ� ��� ���� ����
	if (CAssetMgr::GetInst()->Load<CTexture2D>(_Key, true) != nullptr || !(_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL))
		return E_FAIL;

	ComPtr<ID3D11Texture2D> pDSTex = nullptr;
	if (FAILED(DEVICE->CreateTexture2D(&_Desc, nullptr, pDSTex.GetAddressOf())))
	{
		return E_FAIL;
	}

	m_DSTex = CAssetMgr::GetInst()->CreateTexture(_Key, pDSTex);
	m_DepthClear = _DepthClear;
	m_StencilClear = _StencilClear;
	return S_OK;
}

void CMRT::ClearTargets()
{
	for (int i = 0; i < m_TexCount; ++i)
		CONTEXT->ClearRenderTargetView(m_RTTex[i]->GetRTV().Get(), m_ClearColor[i]);

	if (m_DSTex.Get())
		CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, m_DepthClear, m_StencilClear);
}

void CMRT::SetRenderTarget()
{
	// ����Ʈ ������ ����
	CONTEXT->RSSetViewports(1, &m_Viewport);

	// ��� ����Ÿ�� �� ��� ����Ÿ�� ����
	vector<ID3D11RenderTargetView*> vecRTV;
	for (int i = 0; i < m_TexCount; ++i)
	{
		vecRTV.push_back(m_RTTex[i]->GetRTV().Get());
	}
	CONTEXT->OMSetRenderTargets(m_TexCount, vecRTV.data(), m_DSTex->GetDSV().Get());
}

AssetPtr<CTexture2D> CMRT::GetRenderTarget(int _Idx)
{
	if (_Idx >= m_TexCount)
		return nullptr;

	return m_RTTex[_Idx];
}

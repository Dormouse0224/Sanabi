#pragma once

class CConstBuffer;
class CTexture2D;

class CDevice
	: public Singleton<CDevice>
{
	SINGLE(CDevice);
private:
	HWND								m_hMainWnd;
	D3D11_VIEWPORT						m_ViewPort;
	Vec2								m_RenderResolution;		// ����Ÿ�� �ػ�

	ComPtr<ID3D11Device>				m_Device;				// GPU �޸� ����, DirectX ��ü ����
	ComPtr<ID3D11DeviceContext>			m_Context;				// GPU ��� ����

	ComPtr<IDXGISwapChain>				m_SwapChain;			// Window ȭ�鿡 ������

	AssetPtr<CTexture2D>				m_BackBufferRTT;
	AssetPtr<CTexture2D>				m_BackBufferDST;
	AssetPtr<CTexture2D>				m_RenderTarget[MRT_COUNT];			// �׷��� Ÿ��
	ComPtr<ID3D11RenderTargetView>		m_RTV[MRT_COUNT];
	ComPtr<ID3D11ShaderResourceView>	m_SRV[MRT_COUNT];
	AssetPtr<CTexture2D>				m_DepthStencil;			// ���̰� �׷��� Ÿ��

	CConstBuffer*						m_ConstBuffer[(UINT)CB_TYPE::END];

	ComPtr<ID3D11RasterizerState>		m_RSState[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>		m_DSState[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>			m_BSState[(UINT)BS_TYPE::END];

	ComPtr<ID3D11SamplerState>			m_Sampler[2];			// ���ø�(���� ����)



public:
	int Init(HWND _OutputWnd, Vec2 _vResolution);
	void ClearTarget();
	void Present();

	ComPtr<ID3D11Device> GetDevice() { return m_Device; }
	ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; }
	AssetPtr<CTexture2D> GetRenderTarget(int i);
	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_ConstBuffer[(UINT)_Type]; }
	Vec2 GetRenderResolution() { return m_RenderResolution; }
	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState>	GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }

	void SetRenderTargetAndViewport();
	void SetBackBufferRT();

private:
	int CreateSwapChain();
	int CreateBackBufferView();
	int CreateMRT();
	void CreateConstBuffer();
	void CreateRasterizerState();
	void CreateDepthStencilState();
	void CreateBlendState();
	void CreateSamplerState();
};


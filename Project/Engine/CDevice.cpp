#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"
#include "CTexture2D.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"


CDevice::CDevice()
    : m_hMainWnd(nullptr)
    , m_ConstBuffer{}
    , m_RSState{}
    , m_ViewPort{}
{

}

CDevice::~CDevice()
{
    for (int i = 0; i < (UINT)CB_TYPE::END; ++i)
    {
        if (nullptr != m_ConstBuffer[i])
            delete m_ConstBuffer[i];
    }

    // ��üȭ�� ����
    m_SwapChain->SetFullscreenState(false, nullptr);
}

int CDevice::Init(HWND _OutputWnd, Vec2 _vResolution)
{
    m_hMainWnd = _OutputWnd;
    m_RenderResolution = _vResolution;

    UINT Flag = 0;
#ifdef _DEBUG
    Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
        , Flag, 0, 0, D3D11_SDK_VERSION
        , m_Device.GetAddressOf(), &level, m_Context.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    // ����ü�� ����
    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"SwapChain ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    // View ����
    if (FAILED(CreateBackBufferView()))
    {
        MessageBox(nullptr, L"View ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    // Multi Render Target ����
    CreateMRT();

    // RaterizerState �������� ����
    CreateRasterizerState();

    // DepthStencilState ����
    CreateDepthStencilState();

    // BlendState ����
    CreateBlendState();

    // ������۸� �� ��� �������Ϳ� ������ �� �ֵ��� �ϳ��� ����� ��
    CreateConstBuffer();

    // ViewPort ����
    m_ViewPort.TopLeftX = 0;
    m_ViewPort.TopLeftY = 0;
    m_ViewPort.Width = m_RenderResolution.x;
    m_ViewPort.Height = m_RenderResolution.y;
    m_ViewPort.MinDepth = 0.f;    // ���� �ؽ��Ŀ� �����ϴ� ���̰��� ����
    m_ViewPort.MaxDepth = 1.f;

    SetRenderTargetAndViewport();

    // ���÷� ���� �� ���ε�
    CreateSamplerState();


    return S_OK;
}

void CDevice::ClearTarget()
{
    float rgb = 0.3f;
    if (CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel(); pLevel)
        if (pLevel->GetState() == LEVEL_STATE::PLAY)
            rgb = 0.f;
    float Color[4] = { rgb, rgb, rgb, 1.f };
    m_Context->ClearRenderTargetView(m_BackBufferRTT->GetRTV().Get(), Color);
    m_Context->ClearDepthStencilView(m_BackBufferDST->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
    m_Context->ClearDepthStencilView(m_DepthStencil->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
    for (int i = 0; i < MRT_COUNT; ++i)
    {
        m_Context->ClearRenderTargetView(m_RenderTarget[i]->GetRTV().Get(), Color);
    }
}

void CDevice::Present()
{
    m_SwapChain->Present(0, 0);
}

AssetPtr<CTexture2D> CDevice::GetRenderTarget(int i)
{
    if (i <= MRT_COUNT && i >= 0)
        return m_RenderTarget[i];
    else
        return nullptr;
}

int CDevice::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.OutputWindow = m_hMainWnd;   // ������ ����ü���� �̹����� ��½�ų ������ ������
    Desc.Windowed = true;             // ������ ���, ��üȭ�� ���

    Desc.Flags = 0;
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    Desc.BufferCount = 1;             // ��� Buffer ����         
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Buffer �뵵
    Desc.BufferDesc.Width = (UINT)m_RenderResolution.x;
    Desc.BufferDesc.Height = (UINT)m_RenderResolution.y;
    Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Desc.BufferDesc.RefreshRate.Numerator = 60;
    Desc.BufferDesc.RefreshRate.Denominator = 1;
    Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    ComPtr<IDXGIDevice> pDevice = nullptr;
    ComPtr<IDXGIAdapter> pAdapter = nullptr;
    ComPtr<IDXGIFactory> pFactory = nullptr;

    // ����̽��κ��� ����͸� ���� ���丮�� ����.
    // DXGI ��� �۾������� DXGI ����̽��� �ʿ��ϹǷ� ���� D3D11 ����̽����� ���´�.
    m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);
    pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

    if (FAILED(pFactory->CreateSwapChain(pDevice.Get(), &Desc, m_SwapChain.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int CDevice::CreateBackBufferView()
{
    // RenderTargetView ����
    // �̹� �����Ǿ��ִ� ����Ÿ�� �ؽ��ĸ� ����ü������ ���� ���´�.
    ComPtr<ID3D11Texture2D> pRTTex = nullptr;
    HRESULT hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pRTTex.GetAddressOf());

    m_BackBufferRTT = CAssetMgr::GetInst()->CreateTexture(L"BackBufferRTT", pRTTex);

    // DepthStencil ���ҽ�(�ؽ���) ����    
    D3D11_TEXTURE2D_DESC DSTexDesc = {};

    DSTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DSTexDesc.ArraySize = 1;
    DSTexDesc.Width = (UINT)m_RenderResolution.x;
    DSTexDesc.Height = (UINT)m_RenderResolution.y;
    DSTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    // System Memroy �̵� �Ұ���
    DSTexDesc.Usage = D3D11_USAGE_DEFAULT;
    DSTexDesc.CPUAccessFlags = 0;

    DSTexDesc.MipLevels = 1;
    DSTexDesc.SampleDesc.Count = 1;
    DSTexDesc.SampleDesc.Quality = 0;

    ComPtr<ID3D11Texture2D> pDSTex = nullptr;
    if (FAILED(m_Device->CreateTexture2D(&DSTexDesc, nullptr, pDSTex.GetAddressOf())))
    {
        return E_FAIL;
    }

    m_BackBufferDST = CAssetMgr::GetInst()->CreateTexture(L"BackBufferDST", pDSTex);

    return S_OK;
}

int CDevice::CreateMRT()
{
    // Multi Render Target �ؽ��� ����

    DXGI_FORMAT formats[MRT_COUNT] = {
       DXGI_FORMAT_R8G8B8A8_UNORM,          // Color
       DXGI_FORMAT_R8G8B8A8_UNORM,          // Normal
       DXGI_FORMAT_R8G8B8A8_UNORM           // Position
    };

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.ArraySize = 1;
    texDesc.Width = (UINT)m_RenderResolution.x;
    texDesc.Height = (UINT)m_RenderResolution.y;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    // System Memroy �̵� �Ұ���
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.CPUAccessFlags = 0;

    texDesc.MipLevels = 1;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;

    for (int i = 0; i < MRT_COUNT; ++i)
    {
        texDesc.Format = formats[i];
        ComPtr<ID3D11Texture2D> pMRTTex = nullptr;
        HRESULT res;
        if (FAILED(res = m_Device->CreateTexture2D(&texDesc, nullptr, pMRTTex.GetAddressOf())))
        {
            return E_FAIL;
        }

        m_RenderTarget[i] = CAssetMgr::GetInst()->CreateTexture(L"MRTTexture_" + to_wstring(i), pMRTTex);
        if (m_RenderTarget[i].Get())
        {
            m_RTV[i] = m_RenderTarget[i]->GetRTV().Get();
            m_SRV[i] = m_RenderTarget[i]->GetSRV();
        }
    }

    // MRT �� DSV �ؽ��� ����
    D3D11_TEXTURE2D_DESC DSTexDesc = {};

    DSTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DSTexDesc.ArraySize = 1;
    DSTexDesc.Width = (UINT)m_RenderResolution.x;
    DSTexDesc.Height = (UINT)m_RenderResolution.y;
    DSTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    // System Memroy �̵� �Ұ���
    DSTexDesc.Usage = D3D11_USAGE_DEFAULT;
    DSTexDesc.CPUAccessFlags = 0;

    DSTexDesc.MipLevels = 1;
    DSTexDesc.SampleDesc.Count = 1;
    DSTexDesc.SampleDesc.Quality = 0;

    ComPtr<ID3D11Texture2D> pDSTex = nullptr;
    if (FAILED(m_Device->CreateTexture2D(&DSTexDesc, nullptr, pDSTex.GetAddressOf())))
    {
        return E_FAIL;
    }

    m_DepthStencil = CAssetMgr::GetInst()->CreateTexture(L"MRTDepthStencil", pDSTex);

    return S_OK;
}

void CDevice::CreateConstBuffer()
{
    m_ConstBuffer[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
    m_ConstBuffer[(UINT)CB_TYPE::TRANSFORM]->Create(CB_TYPE::TRANSFORM, sizeof(tTransform));

    m_ConstBuffer[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer;
    m_ConstBuffer[(UINT)CB_TYPE::MATERIAL]->Create(CB_TYPE::MATERIAL, sizeof(tMtrlConst));

    m_ConstBuffer[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer;
    m_ConstBuffer[(UINT)CB_TYPE::GLOBAL]->Create(CB_TYPE::GLOBAL, sizeof(tGlobal));
}

void CDevice::CreateRasterizerState()
{
    m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    D3D11_RASTERIZER_DESC Desc = {};
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
}

void CDevice::CreateDepthStencilState()
{
    m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

    D3D11_DEPTH_STENCIL_DESC Desc = {};

    // Less Equal
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // ���� ��� O
    Desc.StencilEnable = false;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

    // Greater
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_GREATER;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // ���� ��� O
    Desc.StencilEnable = false;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf());

    // NoWrite
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // ���� ��� X
    Desc.StencilEnable = false;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

    // NoTest NoWrite 
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;          // ���� �׻� ���(�˻� X)
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // ���� ��� X
    Desc.StencilEnable = false;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WIRITE].GetAddressOf());
}

void CDevice::CreateBlendState()
{
    // Src : 1, Dst : 0
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;


    // AlphaBlend
    D3D11_BLEND_DESC Desc = {};
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = true;
    for (int i = 0; i < MRT_COUNT; ++i)
    {
        Desc.RenderTarget[i].BlendEnable = true;
        Desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        // RGB ȥ�� ��� ����
        Desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;          // Src �� Dst �� ���ϱ�    
        Desc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;      // Src ����� SrcAlpha
        Desc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // Dst ����� (1 - SrcAlpha)

        // A ȥ�� ��� ����
        Desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ZERO;
        Desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
    }

    DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());
}

void CDevice::CreateSamplerState()
{
    D3D11_SAMPLER_DESC Desc = {};

    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_ANISOTROPIC; // �̹漺 ���͸�
    // MipMap
    Desc.MinLOD = 0;
    Desc.MaxLOD = 0;
    DEVICE->CreateSamplerState(&Desc, m_Sampler[0].GetAddressOf());


    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT; // 2D ����
    // MipMap
    Desc.MinLOD = 0;
    Desc.MaxLOD = 0;
    DEVICE->CreateSamplerState(&Desc, m_Sampler[1].GetAddressOf());

    CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
}

void CDevice::SetRenderTargetAndViewport()
{
    // ����Ʈ ������ ����
    m_Context->RSSetViewports(1, &m_ViewPort);

    // ��� ����Ÿ�� �� ��� ����Ÿ�� ����
    m_Context->OMSetRenderTargets(MRT_COUNT, m_RTV, m_DepthStencil->GetDSV().Get());
}

void CDevice::SetBackBufferRT()
{
    // ����Ʈ ������ ����
    m_Context->RSSetViewports(1, &m_ViewPort);

    // ��� ����Ÿ�� �� ��� ����Ÿ�� ����
    m_Context->OMSetRenderTargets(1, m_BackBufferRTT->GetRTV().GetAddressOf(), m_BackBufferDST->GetDSV().Get());

}

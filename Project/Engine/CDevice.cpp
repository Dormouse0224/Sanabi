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

    // ���÷� ���� �� ���ε�
    CreateSamplerState();


    return S_OK;
}

void CDevice::Present()
{
    m_SwapChain->Present(0, 0);
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
    {
        // 0�� ���� Ÿ�� ���ĺ��� ����
        Desc.RenderTarget[0].BlendEnable = true;
        Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        // RGB ȥ�� ��� ����
        Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;          // Src �� Dst �� ���ϱ�    
        Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;      // Src ����� SrcAlpha
        Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // Dst ����� (1 - SrcAlpha)

        // A ȥ�� ��� ����
        Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    }
    for (int i = 1; i < MRT_COUNT; ++i)
    {
        Desc.RenderTarget[i].BlendEnable = true;
        Desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        // RGB ȥ�� ��� ����
        Desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;          // Src �� Dst �� ���ϱ�    
        Desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;            // Src ����� 1
        Desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;          // Dst ����� 0

        // A ȥ�� ��� ����
        Desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
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

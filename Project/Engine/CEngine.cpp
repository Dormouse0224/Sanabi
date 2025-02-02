#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CTaskMgr.h"
#include "CPhysxMgr.h"

CEngine::CEngine()
    : m_hInst(nullptr)
    , m_hMainWnd(nullptr)
{

}

CEngine::~CEngine()
{
    LiveObjectReport();
}

int CEngine::Init(HINSTANCE _Inst, UINT _Width, UINT _Height)
{
    m_hInst = _Inst;
    m_vResolution = Vec2(_Width, _Height);

    if (FAILED(CreateMainWindow()))
    {
        return E_FAIL;
    }

    // Device 초기화
    if (FAILED(CDevice::GetInst()->Init(m_hMainWnd, m_vResolution)))
    {
        return E_FAIL;
    }

    // Manager 초기화
    CPathMgr::Init();
    CKeyMgr::GetInst()->Init();
    CTimeMgr::GetInst()->Init();
    CAssetMgr::GetInst()->Init();
    CPhysxMgr::GetInst()->Init();
    CLevelMgr::GetInst()->Init();
    CRenderMgr::GetInst()->Init();

    return S_OK;
}

void CEngine::Progress()
{
    // Manager Tick
    CTimeMgr::GetInst()->Tick();
    CKeyMgr::GetInst()->Tick();

    // Level Progress
    CLevelMgr::GetInst()->Progress();

    // Collision Check
    CCollisionMgr::GetInst()->Tick();

    // physx simulate
    CPhysxMgr::GetInst()->Tick();

    // Render
    CRenderMgr::GetInst()->Tick();

    // Task
    CTaskMgr::GetInst()->Tick();
}

int CEngine::CreateMainWindow()
{
    m_hMainWnd = CreateWindowW(L"ClientWindow", L"SNB", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hMainWnd)
        return E_FAIL;

    ShowWindow(m_hMainWnd, true);
    UpdateWindow(m_hMainWnd);

    // 입력된 해상도에 맞는 윈도우 크기 설정
    RECT rt = { 0, 0, (int)m_vResolution.x, (int)m_vResolution.y };
    AdjustWindowRect(&rt, WS_EX_OVERLAPPEDWINDOW, !!GetMenu(m_hMainWnd));
    SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

    return S_OK;
}

void CEngine::LiveObjectReport()
{
    HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
    decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

    IDXGIDebug* debug;

    GetDebugInterface(IID_PPV_ARGS(&debug));

    OutputDebugStringW(L"Starting Live Direct3D Object Dump:\r\n");
    debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
    OutputDebugStringW(L"Completed Live Direct3D Object Dump.\r\n");

    debug->Release();
}
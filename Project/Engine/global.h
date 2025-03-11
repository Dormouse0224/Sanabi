#pragma once


#include <Windows.h>

// ComPtr
#include <wrl/client.h>
using namespace Microsoft::WRL;

// DirectX 라이브러리 링크
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXMath.h>
#include <dxgidebug.h>
#include <dxgi1_3.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "dxgi")

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

// DirectXTex 라이브러리 링크
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_d.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif

// PhysX 라이브러리 링크
#include <physx/PxPhysicsAPI.h>
#ifdef _DEBUG
#pragma comment(lib, "physx/_d/LowLevel_static_64.lib")
#pragma comment(lib, "physx/_d/LowLevelAABB_static_64.lib")
#pragma comment(lib, "physx/_d/LowLevelDynamics_static_64.lib")
#pragma comment(lib, "physx/_d/PhysX_64.lib")
#pragma comment(lib, "physx/_d/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "physx/_d/PhysXCommon_64.lib")
#pragma comment(lib, "physx/_d/PhysXCooking_64.lib")
#pragma comment(lib, "physx/_d/PhysXExtensions_static_64.lib")
#pragma comment(lib, "physx/_d/PhysXFoundation_64.lib")
#pragma comment(lib, "physx/_d/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "physx/_d/PhysXTask_static_64.lib")
#pragma comment(lib, "physx/_d/PhysXVehicle_static_64.lib")
#pragma comment(lib, "physx/_d/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "physx/_d/SceneQuery_static_64.lib")
#pragma comment(lib, "physx/_d/SimulationController_static_64.lib")
#else
#pragma comment(lib, "physx/PhysX_64.lib")
#endif
using namespace physx;

// Visual Leak Detector 라이브러리 링크 (디버그에서만 적용)
#include <VLD/vld.h>

#include <vector>
#include <map>
#include <list>
#include <string>


// 기본 프로젝트 헤더
using namespace std;

#include "define.h"
#include "enum.h"
#include "singleton.h"
#include "struct.h"
#include "func.h"

#include "Ptr.h"


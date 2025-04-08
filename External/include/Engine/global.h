#pragma once

#include <Windows.h>
#define _HAS_STD_BYTE 0		// using namespace std; �� ���� �ذ��
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

// ComPtr
#include <wrl/client.h>
using namespace Microsoft::WRL;

// DirectX ���̺귯�� ��ũ
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

// DirectXTex ���̺귯�� ��ũ
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_d.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif

// PhysX ���̺귯�� ��ũ
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
#pragma comment(lib, "physx/LowLevel_static_64.lib")
#pragma comment(lib, "physx/LowLevelAABB_static_64.lib")
#pragma comment(lib, "physx/LowLevelDynamics_static_64.lib")
#pragma comment(lib, "physx/PhysX_64.lib")
#pragma comment(lib, "physx/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "physx/PhysXCommon_64.lib")
#pragma comment(lib, "physx/PhysXCooking_64.lib")
#pragma comment(lib, "physx/PhysXExtensions_static_64.lib")
#pragma comment(lib, "physx/PhysXFoundation_64.lib")
#pragma comment(lib, "physx/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "physx/PhysXTask_static_64.lib")
#pragma comment(lib, "physx/PhysXVehicle_static_64.lib")
#pragma comment(lib, "physx/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "physx/SceneQuery_static_64.lib")
#pragma comment(lib, "physx/SimulationController_static_64.lib")
#endif
using namespace physx;

// FMOD ���̺귯�� ��ũ
#include <FMOD/fmod.hpp>
#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod_vc.lib")
#endif

// Visual Leak Detector ���̺귯�� ��ũ (����׿����� ����)
#include <VLD/vld.h>

#include <vector>
#include <map>
#include <list>
#include <string>


// ���� ������Ʈ ���
#include "define.h"
#include "enum.h"
#include "singleton.h"
#include "struct.h"
#include "func.h"

#include "AssetPtr.h"


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

// Visual Leak Detector 라이브러리 링크 (디버그에서만 적용)
#include <VLD/vld.h>

#include <vector>
#include <map>
#include <list>
#include <string>

using namespace std;

#include "define.h"
#include "enum.h"
#include "singleton.h"
#include "struct.h"
#include "func.h"

#include "Ptr.h"
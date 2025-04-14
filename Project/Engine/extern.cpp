#include "pch.h"


tTransform g_Trans = {};

tGlobal g_global = {};

const Matrix Matrix::Identity = Matrix(1.f, 0.f, 0.f, 0.f
	, 0.f, 1.f, 0.f, 0.f
	, 0.f, 0.f, 1.f, 0.f
	, 0.f, 0.f, 0.f, 1.f);

wstring COMPONENT_TYPE_WSTR[] =
{
	L"TRANSFORM",
	L"CAMERA",
	L"PHYSXACTOR",
	L"FSM",
	L"UICOM",
	L"COLLIDER3D",
	L"BOUNDINGBOX",


	L"MESHRENDER",
	L"SPRITERENDER",
	L"FLIPBOOKRENDER",
	L"TILERENDER",
	L"PARTICLERENDER",
	L"SKYBOX",
	L"DECAL",
	L"LANDSCAPE",

	L"COMPONENT_END",

	L"SCRIPT",
};
static_assert(sizeof(COMPONENT_TYPE_WSTR) / sizeof(COMPONENT_TYPE_WSTR[0]) == static_cast<int>(COMPONENT_TYPE::COMPONENT_END) + 2);

wstring ASSET_TYPE_WSTR[] = 
{
	L"MESH",
	L"MESHDATA",
	L"TEXTURE",
	L"MATERIAL",
	L"PREFAB",
	L"SOUND",
	L"GRAPHIC_SHADER",	// ·»´õ¸µ
	L"COMPUTE_SHADER", // °è»ê, GP(General Purpose)
	L"SPRITE",
	L"FLIPBOOK",
	L"FSM_STATE",
	L"FONT",
};
static_assert(sizeof(ASSET_TYPE_WSTR) / sizeof(ASSET_TYPE_WSTR[0]) == static_cast<int>(ASSET_TYPE::ASSET_END));

wstring COLLISION_LAYER_WSTR[] =
{
	L"ePLAYER",
	L"eLANDSCAPE",
	L"eMONSTER",
	L"ePARTICLE",

};
static_assert(sizeof(COLLISION_LAYER_WSTR) / sizeof(COLLISION_LAYER_WSTR[0]) == COLLISION_LAYER::END);

wstring LAYER_WSTR[] =
{
	L"Default",
	L"Background",
	L"Tile",
	L"Player",
	L"PlayerProjectile",
	L"Enermy",
	L"EnermyProjectile",

	L"UI",
};
static_assert(sizeof(LAYER_WSTR) / sizeof(LAYER_WSTR[0]) == static_cast<int>(LAYER::END));
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
	L"COLLIDER2D",
	L"PHYSXACTOR",
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

wstring CAMERA_LAYER_WSTR[] =
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
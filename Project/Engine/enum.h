#pragma once

enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2

	END,
};


// Rasterizer State
enum class RS_TYPE
{
	CULL_BACK,	// cull_ccw, �޸�(�ݽð� ����) �ø�
	CULL_FRONT, // cull_cw
	CULL_NONE,  // �ø����� ����
	WIRE_FRAME, // �ø����� ����, 

	END,
};

// DepthStencilState
enum class DS_TYPE
{
	LESS,				// �������� : �� �۾ƾ� ���, ���� ��� O
	LESS_EQUAL,			// �������� : �� �۾ƾ� ������ ���, ���� ��� O

	GREATER,			// �������� : �� �־�� ���, ���� ��� 0

	NO_WRITE,			// �������� : �� �۾ƾ� ���(LESS), ���� ��� X
	NO_TEST_NO_WIRITE,	// �������� �׻� ����(�˻�X), ���� ��� X

	END,
};

// BlendState
enum class BS_TYPE
{
	DEFAULT,	// Src : 1, Dst : 0
	ALPHABLEND, // Src : A, Dst : (1 - A)
	ALPHABLEND_COVERAGE,
	ONE_ONE,

	END,
};

// wstr ����ü ������Ʈ �� ��
enum class COMPONENT_TYPE
{
	TRANSFORM,
	CAMERA,
	PHYSXACTOR,
	FSM,
	UICOM,
	COLLIDER3D,
	BOUNDINGBOX,

	// Rendering Component
	MESHRENDER,			// ���� �⺻���� RenderComponent
	SPRITERENDER,		// Sprite �ϳ� �������ϴ� ������Ʈ
	FLIPBOOKRENDER,		// Flipbook ��� ������Ʈ
	TILERENDER,			// Ÿ�ϸ� ������
	PARTICLERENDER,		// ���� ������
	SKYBOX,				// 3D ���
	DECAL,				// ����
	LANDSCAPE,			// 3D ����

	// Additional
	LIGHT,

	COMPONENT_END,

	SCRIPT,				// �뺻, ����, ������
};
extern std::wstring COMPONENT_TYPE_WSTR[];

enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	TEXTURE2D,
	MATERIAL,
	PREFAB,
	SOUND,
	GRAPHIC_SHADER,	// ������
	COMPUTE_SHADER, // ���, GP(General Purpose)
	SPRITE,
	FLIPBOOK,
	FSM_STATE,
	FONT,

	ASSET_END,
};
extern std::wstring ASSET_TYPE_WSTR[];


// ����(Projection) ���
enum PROJ_TYPE
{
	ORTHOGRAPHIC,	// ��������
	PERSPECTIVE,	// ��������
};


// ����
enum class DIR
{
	RIGHT,
	UP,
	FRONT,
	END,
};

enum class SHADER_DOMAIN
{
	DOMAIN_QPAQUE,		// ������
	DOMAIN_MASKED,		// ������ + ���� ����
	DOMAIN_TRANSPARENT, // ������ + ����
	DOMAIN_POSTPROCESS,	// ��ó��
	DOMAIN_UI,			// UI
	DOMAIN_DEBUG,		// ����� ������
	DOMAIN_SYSTEM,		// ī�޶� �з��� ������ ����
};

enum CONST_PARAM
{
	INT_0,		INT_1,		INT_2,		INT_3,
	FLOAT_0,	FLOAT_1,	FLOAT_2,	FLOAT_3,
	VEC2_0,		VEC2_1,		VEC2_2,		VEC2_3,
	VEC4_0,		VEC4_1,		VEC4_2,		VEC4_3,
	MAT_0,		MAT_1,		MAT_2,		MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEX_END,
};


enum class DEBUG_SHAPE
{
	// 2D
	RECT,
	CIRCLE,
	LINE,

	// 3D
	CUBE,
	SPHERE,
};

enum COLLISION_LAYER
{
	ePLAYER				= (1<<0),
	eLANDSCAPE			= (1<<1),
	eMONSTER			= (1<<2),
	ePARTICLE			= (1<<3),

	END = 4
};
extern std::wstring COLLISION_LAYER_WSTR[COLLISION_LAYER::END];

// wstr ����ü ������Ʈ �� ��
enum class LAYER
{
	Default,
	Background,
	Tile,
	Player,
	PlayerProjectile,
	Enemy,
	EnemyProjectile,

	UI,

	END
};
extern std::wstring LAYER_WSTR[static_cast<int>(LAYER::END)];

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// ���⼺ ����, ���� (�¾�, ��)
	POINT,			// ������, ����, ����, ȶ��
	SPOT,			// ����Ʈ����Ʈ, ������

	END
};
extern std::wstring LIGHT_TYPE_WSTR[static_cast<int>(LIGHT_TYPE::END)];

enum MRT_TYPE
{
	DEFERRED,
	MERGE,

	MRT_END
};
#pragma once



// Vertex
// ��ü�� ���¸� �̷�� ���� �⺻ ����
struct Vtx
{
	Vec3	vPos;		// ������ ��ġ
	Vec4	vColor;		// ������ ����	
	Vec2	vUV;		// ������ ������ �ؽ����� ��ġ(��ǥ)
};

// DebugShapeInfo
struct tDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vec3	    WorldPos;
	Vec3		WorldScale;
	Vec4		WorldRotation;
	Matrix		MatWorld;
	Vec4		Color;
	float		CurTime;
	float		Duration;
	bool		DepthTest;
};

struct tTask
{
	TASK_TYPE Type;
	DWORD_PTR Param0;
	DWORD_PTR Param1;
};


// =============
// Particle ����
// =============
struct tParticle
{
	Vec4		LocalPos;		// ���� ������Ʈ�� ������ ��� ��ǥ (World �������� ��� ���̴����� ObjectPos �� ����)
	Vec4		WorldScale;		// ��ƼŬ ���� ũ��
	Vec4		Color;			// ��ƼŬ ����

	Vec4		Force;			// ��ƼŬ�� �־����� �ִ� ���� ����
	Vec4		Velocity;		// ��ƼŬ �ӵ�
	float		Mass;			// ��ƼŬ ����

	float		Age;			// ��ƼŬ ����, Age �� Life �� �����ϸ� ������ ���� ��
	float		Life;			// ��ƼŬ �ִ� ����
	float		NormalizedAge;  // ��ü ���� ���, ���� Age ����. �ڽ��� Age �� Life ��� ����ȭ �� ��

	int			Active;			// ��ƼŬ Ȱ��ȭ ����
	int			PrevActive;		// ���� �����ӿ��� ��ƼŬ Ȱ��ȭ ����

	UINT		EntityID;

	int			padding;
};

struct tSpawnCount
{
	int		SpawnCount;
	int		padding[3];
};


// Particle Module
struct tParticleModule
{
	// Spawn Modlue
	float	SpawnRate;			// �ʴ� ��ƼŬ ������
	Vec4	StartColor;			// �ʱ� ��ƼŬ ����
	Vec4	EndColor;			// ��ƼŬ ���� ����
	Vec3	MinScale;			// ���� �� �ּ� ũ��
	Vec3	MaxScale;			// ���� �� �ִ� ũ��
	float	MinSpeed;			// ���� �� �ּ� �ӷ�
	float	MaxSpeed;			// ���� �� �ִ� �ӷ�
	float	MinLife;			// ���� �� �ּ� �ֱ�
	float	MaxLife;			// ���� �� �ִ� �ֱ�
	int		SpawnShape;			// 0 : Box, 1 : Sphere
	Vec3	SpawnShapeScale;
	Vec3	SpawnDir;
	float	SpawnDirRandomize;	// 0 ~ 1, ���� ����ȭ ����
	int		SpaceType;			// 0 : Local, 1 : World

	// �߰� ������
	Vec3	ObjectWorldPos;

	int		padding;
};





// ===================
// ������� ���� ����ü
// ===================

struct tTransform
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;

	Matrix	matWV;
	Matrix	matVP;
	Matrix	matWVP;
};
extern tTransform g_Trans;

// ������ ���ؼ� ���޵Ǵ� ���
struct tMtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	matArr[4];

	//�ؽ��� ���ε� ����
	int		bTex[TEX_END];

	int		padding[2];
};

// Global Data
struct tGlobal
{
	Vec2	g_Resolution;
	float   g_DT;
	float   g_EngineDT;
	float   g_AccTime;
	int     g_Light2DCount;
	int     g_Light3DCount;

	int		padding;
};
extern tGlobal g_global;
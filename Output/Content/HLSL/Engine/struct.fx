#ifndef _STRUCT
#define _STRUCT

// Particle
struct tParticle
{
    float4 LocalPos; // ���� ������Ʈ�� ������ ��� ��ǥ
    float4 WorldScale; // ��ƼŬ ���� ũ��
    float4 Color; // ��ƼŬ ����

    float4 Force; // ��ƼŬ�� �־����� �ִ� ���� ����
    float4 Velocity; // ��ƼŬ �ӵ�
    float Mass; // ��ƼŬ ����

    float Age; // ��ƼŬ ����, Age �� Life �� �����ϸ� ������ ���� ��
    float Life; // ��ƼŬ �ִ� ����
    float NormalizedAge; // ��ü ���� ���, ���� Age ����. �ڽ��� Age �� Life ��� ����ȭ �� ��

    int Active; // ��ƼŬ Ȱ��ȭ ����
    int PrevActive; // ���� �����ӿ��� ��ƼŬ Ȱ��ȭ ����
    
	uint EntityID;
    
    int padding; // ��ƼŬ ����ü ũ�⸦ 16 ����� ���߱� ���� �е� ����Ʈ
};

// SpawnCount
struct tSpawnCount
{
    int SpawnCount;
    int3 padding;
};

// Particle Module
struct tParticleModule
{
    // Spawn Module
    float SpawnRate; // �ʴ� ��ƼŬ ������
    float4 StartColor; // �ʱ� ��ƼŬ ����
    float4 EndColor; // ��ƼŬ ���� ����
    float3 MinScale; // ���� �� �ּ� ũ��
    float3 MaxScale; // ���� �� �ִ� ũ��
    float MinSpeed; // ���� �� �ּ� �ӷ�
    float MaxSpeed; // ���� �� �ִ� �ӷ�
    float MinLife; // ���� �� �ּ� �ֱ�
    float MaxLife; // ���� �� �ִ� �ֱ�
    int SpawnShape; // 0 : Box, 1 : Sphere
    float3 SpawnShapeScale;
    float3 SpawnDir;
    float SpawnDirRandomize; // 0 ~ 1, ���� ����ȭ ����
    int SpaceType; // 0 : Local, 1 : World
    int VelocityAllignment;
            
    // �߰� ������
    float3 ObjectWorldPos;
    
    //int padding;
};


struct tLightModule
{
    int Type; // ���� ����
    float3 Color; // ���� ����
    float3 WorldPos; // ���� ��ġ	
    float Radius; // ���� ����, �ݰ�
    float3 Dir; // ������ ����
    float Angle; // ���� ����(����Ʈ ����Ʈ)	
};

#endif 
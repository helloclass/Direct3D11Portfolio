#pragma once
#include <assert.h>
#include "GlobalDataStruct.h"

class Particle
{
public:
	struct SpaceRange
	{
		float3 MinBorder;
		float3 MaxBorder;
	};

public:
	struct MainSetting
	{
		float Duration;
		bool bIsLooping;

		// ���� ����
		float StartDelay;
		float StartLifeTime;

		float3 Velocity;
		float3 Rotation;
		float3 Size;
		float3 Color;

		bool bIsPlayOnAwake;

		int MaxParticleNum;
	};
	// Shape�� Shape ���� ���� ������ �������� ��ƼŬ�� ���� �� �ִ� SPAN�� ��Ÿ���� ��ƼŬ ���� �����̴�. 
	struct ShapeSetting
	{
		// For Random Initialize
		SpaceRange PositionRange;
		SpaceRange RotaionRange;
		SpaceRange ScaleRange;

		SpaceRange Velocity;
		SpaceRange Torque;
	};
	struct NoiseSetting
	{
		ID3D11ShaderResourceView* ParticleNoiseTexture;

		float3 NoisePosition;
		float3 NoiseRotation;
		float3 NoiseScale;

		float3 NoiseVelocity;
		float3 NoiseTorque;
	};
	struct OptionalSetting
	{
		// ��ƼŬ ������ �ּ� �Ÿ�
		float RestOffset;
		// ���� ����
		float AirFictionScale;
		// �ܺ� ����
		float3 Accumulate;

		// ����
		//ThreadPool[workerIter].ParticleSystem->setExternalAcceleration();
		//ThreadPool[workerIter].ParticleSystem->setRestOffset();

		// ���� ���� (���� ����, ������ ��)
		//ThreadPool[workerIter].ParticleSystem->setDynamicFriction();
	};

public:
	MainSetting main;
	ShapeSetting shape;
	NoiseSetting noise;
	OptionalSetting optional;

public:
	Particle() = delete;
	Particle(int MaxParticleNum)
	{
		main.MaxParticleNum = MaxParticleNum;

		main.Duration		= 3.0f;
		main.bIsLooping		= false;

		main.StartDelay		= 0.0f;
		main.StartLifeTime	= 3.0f;

		main.Velocity		= float3(0.0f, 0.0f, 0.0f);
		main.Rotation		= float3(0.0f, 0.0f, 0.0f);
		main.Size			= float3(0.0f, 0.0f, 0.0f);
		main.Color			= float3(0.0f, 0.0f, 0.0f);

		main.bIsPlayOnAwake = true;

		shape.PositionRange.MinBorder	= float3(0.0f, 0.0f, 0.0f);
		shape.RotaionRange.MinBorder	= float3(0.0f, 0.0f, 0.0f);
		shape.ScaleRange.MinBorder		= float3(1.0f, 1.0f, 1.0f);

		shape.PositionRange.MaxBorder	= float3(0.0f, 0.0f, 0.0f);
		shape.RotaionRange.MaxBorder	= float3(0.0f, 0.0f, 0.0f);
		shape.ScaleRange.MaxBorder		= float3(1.0f, 1.0f, 1.0f);

		shape.Velocity.MinBorder		= float3(0.0f, 0.0f, 0.0f);
		shape.Torque.MinBorder			= float3(0.0f, 0.0f, 0.0f);

		shape.Velocity.MaxBorder		= float3(0.0f, 0.0f, 0.0f);
		shape.Torque.MaxBorder			= float3(0.0f, 0.0f, 0.0f);

		noise.NoisePosition				= float3(0.0f, 0.0f, 0.0f);
		noise.NoiseRotation				= float3(0.0f, 0.0f, 0.0f);
		noise.NoiseScale				= float3(1.0f, 1.0f, 1.0f);

		noise.NoiseVelocity				= float3(0.0f, 0.0f, 0.0f);
		noise.NoiseTorque				= float3(0.0f, 0.0f, 0.0f);

		optional.RestOffset				= 0.0f;
		optional.AirFictionScale		= 0.0f;
		optional.Accumulate				= float3(0.0f, 0.0f, 0.0f);
	}

};

struct ParticleWorker
{
	DWORD	UpdateThreadID = 0;
	HANDLE	UpdateThreadFunc = nullptr;

	HANDLE ParticleReadEvent;
	HANDLE ParticleWriteEvent;

	physx::PxParticleSystem* ParticleSystem;

	// ��ũ���� ���� �� ��ƼŬ ���� ����
	// �����忡�� �����޸𸮷� ����� �����̱⿡ ���޸𸮿� ����
	std::vector<PxU32>		ParticleIndexBuffer;
	std::vector<PxVec3>		ParticlePositionBuffer;
	std::vector<PxVec3>		ParticleVelocityBuffer;

	Particle*	ParticleDataDesc;

	// �ε��� Ǯ ����
	PxParticleExt::IndexPool* ParticleIndexPool;

public:
	ParticleWorker() = delete;
	ParticleWorker(int MaxParticleNum)
	{
		ParticleDataDesc = new Particle(MaxParticleNum);
	};

	~ParticleWorker()
	{

	};
};

class ParticlePhysics : public Physics
{
public:
	// Particle�� �� ����޽� ������ MaxParticle�� ������ ��Ÿ����.
	std::vector<unsigned>			ParticleNumList;

	std::vector<ParticleWorker*>	ThreadPool;

public:
	ParticlePhysics() = delete;
	ParticlePhysics(std::vector<unsigned>& ParticleNum);

	~ParticlePhysics();

public:
	void ParticleUpdate();
};
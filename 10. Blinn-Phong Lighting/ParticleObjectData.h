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

		// 선형 감소
		float StartDelay;
		float StartLifeTime;

		float3 Velocity;
		float3 Rotation;
		float3 Size;
		float3 Color;

		bool bIsPlayOnAwake;

		int MaxParticleNum;
	};
	// Shape란 Shape 영역 내의 공간에 랜덤으로 파티클이 생길 수 있는 SPAN을 나타내는 파티클 생성 영역이다. 
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
		// 파티클 사이의 최소 거리
		float RestOffset;
		// 공기 마찰
		float AirFictionScale;
		// 외부 가속
		float3 Accumulate;

		// 가속
		//ThreadPool[workerIter].ParticleSystem->setExternalAcceleration();
		//ThreadPool[workerIter].ParticleSystem->setRestOffset();

		// 공기 마찰 (생일 폭죽, 배드민턴 공)
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

	// 디스크립션 제작 및 파티클 공간 생성
	// 쓰레드에서 공유메모리로 사용할 버퍼이기에 힙메모리에 생성
	std::vector<PxU32>		ParticleIndexBuffer;
	std::vector<PxVec3>		ParticlePositionBuffer;
	std::vector<PxVec3>		ParticleVelocityBuffer;

	Particle*	ParticleDataDesc;

	// 인덱스 풀 생성
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
	// Particle의 각 서브메쉬 단위로 MaxParticle의 개수를 나타낸다.
	std::vector<unsigned>			ParticleNumList;

	std::vector<ParticleWorker*>	ThreadPool;

public:
	ParticlePhysics() = delete;
	ParticlePhysics(std::vector<unsigned>& ParticleNum);

	~ParticlePhysics();

public:
	void ParticleUpdate();
};
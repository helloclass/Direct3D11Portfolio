#pragma once
#include "DirectX.h"

class Axis : public ParticleObjectData
{
private:
	std::string ObjectName = "Axis";
	std::string ObjectPath = "Resource\\axis.fbx";

	UIObjectData* particle = nullptr;
	UIObjectData* particle2 = nullptr;
	UIObjectData* particle3 = nullptr;
	UIObjectData* particle4 = nullptr;

public:
	Axis() {
		ParticleObjectData::ParticleObjectData();

		fbxLoader.CreateFBXModel(
			this,
			ObjectName,
			ObjectPath
		);

		ShaderID = "Axis.hlsl";
		bIsEnableDepthStencil = false;
	}

	void Awake() {
		ParticleObjectData::Awake();

		particle	= new UIObjectData();
		particle2	= new UIObjectData();
		particle3	= new UIObjectData();
		particle4	= new UIObjectData();

		// load Particle 1
		fbxLoader.LoadFBXModel(
			particle,
			ObjectName,
			ObjectPath
		);
		particle->Instantiate(200);
		Hierarchy.Insert(&particle->Hierarchy);

		// load Particle 2
		fbxLoader.LoadFBXModel(
			particle2,
			ObjectName,
			ObjectPath
		);
		particle2->Instantiate(30);
		Hierarchy.Insert(&particle2->Hierarchy);

		// load Particle 3
		fbxLoader.LoadFBXModel(
			particle3,
			ObjectName,
			ObjectPath
		);
		particle3->Instantiate(40);
		Hierarchy.Insert(&particle3->Hierarchy);

		// load Particle 4
		fbxLoader.LoadFBXModel(
			particle4,
			ObjectName,
			ObjectPath
		);
		particle4->Instantiate(50);
		Hierarchy.Insert(&particle4->Hierarchy);


	}

	void Start() {
		ParticleObjectData::Start();
	}

	void Update() {
		ParticleObjectData::Update();

	}

	void Destroy() {
		ParticleObjectData::Destroy();

		delete particle;
		delete particle2;
		delete particle3;
		delete particle4;
	}
};
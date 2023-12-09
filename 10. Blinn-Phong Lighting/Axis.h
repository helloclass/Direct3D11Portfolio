#pragma once
#include "DirectX.h"

class Axis : public UIObjectData
{
private:
	std::string ObjectName = "Axis";
	std::string ObjectPath = "Resource\\axis.fbx";

public:
	Axis() {
		UIObjectData::UIObjectData();

		fbxLoader.CreateFBXModel(
			this,
			ObjectName,
			ObjectPath
		);

		ShaderID = "Axis.hlsl";
		bIsEnableDepthStencil = false;
	}

	void Awake() {
		UIObjectData::Awake();
	}

	void Start() {
		UIObjectData::Start();

	}

	void Update() {
		UIObjectData::Update();

	}

	void Destroy() {
		UIObjectData::Destroy();

	}
};
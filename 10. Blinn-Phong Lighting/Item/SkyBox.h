#pragma once
#include "DirectX.h"

class SkyBox : public UIObjectData
{
private:
	std::string ObjectName = "Skybox";
	std::string ObjectPath = "Resource\\skybox.fbm\\skybox.fbx";

public:
	SkyBox() {
		UIObjectData::UIObjectData();

		fbxLoader.CreateFBXModel(
			this,
			ObjectName,
			ObjectPath
		);

		ShaderID = "SkyBox.hlsl";
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
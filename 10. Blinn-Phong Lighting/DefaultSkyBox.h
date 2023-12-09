#pragma once
#include "DirectX.h"

class DefaultSkyBox : public UIObjectData
{
private:
	std::string ObjectName = "DefaultSkybox";
	std::string ObjectPath = "Resource\\defaultSkybox.FBX";

public:
	DefaultSkyBox() {
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


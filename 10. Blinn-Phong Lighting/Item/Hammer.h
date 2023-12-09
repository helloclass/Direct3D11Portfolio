#pragma once
#include "../DirectX.h"

class Hammer : public DynamicObjectData
{
private:
	std::string ObjectName = "Weapon";
	std::string ObjectPath = "Resource\\Hammer_04.fbx";

public:
	Hammer() {
		DynamicObjectData::DynamicObjectData();

		fbxLoader.CreateFBXSkinnedModel(
			this,
			ObjectName,
			ObjectPath
		);

#pragma region ImportTexture

		// 텍스쳐 로드하는거 만들기

		MemoryPool::getInstance().TexturePaths.insert("Resource\\P_970084.png");
		MemoryPool::getInstance().TexturePaths.insert("Resource\\U_970084.png");

		ShaderID = "Hammer.hlsl";

		// 텍스쳐 삽입 지정은 사람이 집적 입력
		meshDatas[0].mMaterial.BaseColorTexutures.push_back(
			Path2FileName("Resource\\P_970084.png")
		);
		// Mask
		meshDatas[0].mMaterial.MaskedTextures.push_back(
			Path2FileName("Resource\\U_970084.png")
		);

#pragma endregion

#pragma region ExportAnim

		//std::string Root = "Sequence\\Idle\\anim";
		//std::vector<std::string> AnimationsPathList;

		//GetAllSubFolders(Root, AnimationsPathList);

		//ObjectData* Copier = nullptr;
		//for (int i = 0; i < AnimationsPathList.size(); i++)
		//{
		//	Copier = fbxLoader.LoadFBXSkinnedModel(
		//		Path2FileName(AnimationsPathList[i].c_str()),
		//		AnimationsPathList[i]
		//	);

		//	ExportAnimationSequence(Copier);

		//	delete (Copier);
		//}

#pragma endregion

#pragma region ExportMorph

//// 다양한 몸체가 있는 폴더
//std::string Root = "Sequence\\Idle\\anim";
//std::vector<std::string> AnimationsPathList;
//
//GetAllSubFolders(Root, AnimationsPathList);
//
//ObjectData* Copier = nullptr;
//for (int i = 0; i < AnimationsPathList.size(); i++)
//{
//	Copier = fbxLoader.LoadFBXSkinnedModel(
//		Path2FileName(AnimationsPathList[i].c_str()),
//		AnimationsPathList[i]
//	);
//
//	ExportAnimationMorph(Copier);
//
//	delete (Copier);
//}

#pragma endregion

#pragma region ImportAnim

		//std::string root = "MetaData\\" + ObjectName + "\\Sequence";
		//std::string path;

		//for (const auto& file : directory_iterator(root))
		//{
		//	path = file.path().string();

		//	ImportAnimationSequence(path);
		//}

#pragma endregion

	}

	void Awake() {
		DynamicObjectData::Awake();

	}

	void Start() {
		DynamicObjectData::Start();

		transform->InstanceBuffer.begin()->second->SetRotation(0.0f, 0.0f, 0.0f);

	}

	void Update() {
		DynamicObjectData::Update();

	}

	void Destroy() {
		DynamicObjectData::Destroy();

	}

public:
	std::string getName()
	{
		return ObjectName;
	}
};
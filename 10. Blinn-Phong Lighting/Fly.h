#pragma once
#include "DirectX.h"

class Fly : public DynamicObjectData
{
private:
	std::string ObjectName = "ME";
	std::string ObjectPath = "Sequence\\Idle\\Idle.fbx";

public:
	Fly() {
		DynamicObjectData::DynamicObjectData();

		fbxLoader.CreateFBXSkinnedModel(
			this,
			ObjectName,
			ObjectPath
		);

#pragma region ImportTexture

		// 텍스쳐 로드하는거 만들기

		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\N_182752.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\P_182752.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\U_182752.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\Female_body_Default_NM.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\Female_body_Dry_NM.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\Female_body_Mus_NM.png");

		ShaderID = "Character_Body.hlsl";

		// 텍스쳐 삽입 지정은 사람이 집적 입력
		meshDatas[0].mMaterial.BaseColorTexutures.push_back(
			Path2FileName("Sequence\\Idle\\P_182752.png")
		);
		// BodyNormal
		meshDatas[0].mMaterial.NormalTexutures.push_back(
			Path2FileName("Sequence\\Idle\\Female_body_Default_NM.png")
		);
		meshDatas[0].mMaterial.NormalTexutures.push_back(
			Path2FileName("Sequence\\Idle\\N_182752.png")
		);
		meshDatas[0].mMaterial.MaskedTextures.push_back(
			Path2FileName("Sequence\\Idle\\U_182752.png")
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

		std::string root = "MetaData\\" + ObjectName + "\\Sequence";
		std::string path;

		for (const auto& file : directory_iterator(root))
		{
			path = file.path().string();

			ImportAnimationSequence(path);
		}

#pragma endregion

	}

	void Awake() {
		DynamicObjectData::Awake();

	}

	void Start() {
		DynamicObjectData::Start();

		// Sequence Test
		//ExportAnimationSequence();
		//ImportAnimationSequence(std::string("MetaData\\ME\\Sequence0.seq"));
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
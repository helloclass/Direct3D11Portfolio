#pragma once
#include "../DirectX.h"

class Fly : public DynamicObjectData
{
private:
	std::string ObjectName = "Body";
	std::string ObjectPath = "Sequence\\Idle\\Idle.fbx";

public:
	Fly() {
		DynamicObjectData::DynamicObjectData();

		fbxLoader.CreateFBXSkinnedModel(
			this,
			ObjectName,
			ObjectPath
		);

		geo::CreateBox("ScreenBox", 2.0f, 2.0f, 1.0f);

#pragma region ImportTexture

		// �ؽ��� �ε��ϴ°� �����

		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\N_182752.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\P_182752.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\U_182752.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\Female_body_Default_NM.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\Female_body_Dry_NM.png");
		MemoryPool::getInstance().TexturePaths.insert("Sequence\\Idle\\Female_body_Mus_NM.png");

		ShaderID = "Character_Body.hlsl";

		// �ؽ��� ���� ������ ����� ���� �Է�
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

//// �پ��� ��ü�� �ִ� ����
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

		//transform->InstanceBuffer.begin()->second->SetRotation(0.0f, 0.0f, 90.0f);

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
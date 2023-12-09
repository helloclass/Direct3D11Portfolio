#pragma once
#include "../DirectX.h"

class Head : public DynamicObjectData
{
private:
	std::string ObjectName = "Head";
	std::string ObjectPath = "Resource\\Head\\Head1.fbx";

public:
	Head() {
		DynamicObjectData::DynamicObjectData();

		fbxLoader.CreateFBXSkinnedModel(
			this,
			ObjectName,
			ObjectPath
		);

#pragma region ImportTexture

		// �ؽ��� �ε��ϴ°� �����

		MemoryPool::getInstance().TexturePaths.insert("Resource\\Head\\HF_FaceColor_nose_.png");
		//MemoryPool::getInstance().TexturePaths.insert("Resource\\Head\\Tatto_Face12.png");

		ShaderID = "Head.hlsl";

		// �ؽ��� ���� ������ ����� ���� �Է�

		// 0 ���� ����޽��� meshDatas�� ���� �� �ֵ��� ������Ʈ
		meshDatas[0].mMaterial.BaseColorTexutures.push_back(
			Path2FileName("Resource\\Head\\HF_FaceColor_nose_.png")
		);
		meshDatas[1].mMaterial.BaseColorTexutures.push_back(
			Path2FileName("Resource\\Head\\HF_FaceColor_nose_.png")
		);
		meshDatas[2].mMaterial.BaseColorTexutures.push_back(
			Path2FileName("Resource\\Head\\HF_FaceColor_nose_.png")
		);
		meshDatas[3].mMaterial.BaseColorTexutures.push_back(
			Path2FileName("Resource\\Head\\HF_FaceColor_nose_.png")
		);

		//meshDatas[0].mMaterial.MaskedTextures.push_back(
		//	Path2FileName("Resource\\Head\\Tatto_Face12.png")
		//);

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
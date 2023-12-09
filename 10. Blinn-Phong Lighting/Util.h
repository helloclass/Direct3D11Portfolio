#pragma once

#include <windows.h>
#include <windowsx.h>

#include <commctrl.h>
#include <memory>
#include <string>
#include <stdexcept>
#include <assert.h>
#include <wrl.h>
#include <fstream>
#include <mutex>
#include <filesystem>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "AnimationAsset.h"

using std::filesystem::directory_iterator;

class ObjectData
{
public:

	// Initialize
	ObjectData() {
		objType = ObjectType::OBJ_TYPE;
		objectName = "";

		transform = new Transform();

		Hierarchy.Item = this;
	}

	// Destroy
	~ObjectData() {
		delete (transform);

		for (int idx = 0; idx < Sequences.size(); idx++)
		{
			delete (Sequences[idx]);
		}
	}

public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void Destroy() {}

public:
	// Max InstanceCount is 2000 
	virtual void Instantiate() {}
	virtual void Instantiate(UINT Number) {}
	virtual void Instantiate(Instance& t) {}
	virtual void Instantiate(std::string Name) {}
	virtual void Instantiate(float3 Position) {}

	virtual void Destroy(std::string name) {}

public:
	ObjectType					objType;
	PhysicsType					physicsType;

	std::string					objectName;

	std::vector<MeshData>		meshDatas;
	std::vector<FbxMesh*>		meshFBXDatas;
	Material					meshMaterial;

	DirectX::BoundingBox		collider;
	MeshData					meshBoundBoxData;

	UINT						SubmeshCount;
	std::vector<ID3D11Buffer*>	VertexBuffer;
	std::vector<ID3D11Buffer*>	IndexBuffer;

	ID3D11Buffer*				VertexBoundBoxBuffer;
	ID3D11Buffer*				IndexBoundBoxBuffer;

	UINT						NumIndices;
	UINT						Stride;

	UINT						VerticesOffset;
	UINT						IndicesOffset;

	// Instance Buffer
	ID3D11Buffer*				InstanceBuffer;

	// Shader Buffer
	std::string					ShaderID = "BlinnPhong.hlsl";

	ID3D11Buffer*				LightVSConstantBuffer;
	ID3D11Buffer*				ObjectVSConstantBuffer;
	ID3D11Buffer*				ObjectPSConstantBuffer;

	ID3D11Buffer*				ScreenSizeVSConstantBuffer;

	fbxsdk::FbxAMatrix			pGlobalPosition;

	Transform*					transform;

public:
	// Object Hierarchy
	Tree<ObjectData*>			Hierarchy;

	std::string  ParentRootBoneName;
	std::string  ParentBindBoneName;

	// �θ� ������Ʈ�� ž�� ��Ų��
	bool AttachTo(ObjectData* Parent, std::string RootSocketName)
	{
		if (!Parent)
			return false;

		// �θ� ���� ã��
		ParentRootBoneName = RootSocketName;

		// �θ� ���̾��Ű�� �ڽ� ������Ʈ Ʈ�� ����
		Parent->Hierarchy.Insert(&Hierarchy);
		// �� ������Ʈ�� ��Ʈ�� Parent Ʈ�� ����
		Hierarchy.Parent = &Parent->Hierarchy;
		// ��Ʈ == �θ� Ʈ��
		Hierarchy.Root = Parent->Hierarchy.Root;

		return true;
	}

	bool BindSocketTo(std::string ParentSocketName)
	{
		if (ParentRootBoneName.empty())
			return false;

		ParentBindBoneName = ParentSocketName;

		return true;
	}

	// ���� ���� �ٷ� �� �θ������ Transform�� ��� ���Ͽ� ����
	float4x4 GetParentTranformRecursion(Tree<ObjectData*>& node)
	{
		float4x4 matrix;
		transform->InstanceBuffer.begin()->second->GetLocalAffineMatrix(matrix);

		if (node.Parent)
			return GetParentTranformRecursion(*node.Parent) * matrix;

		return matrix;
	}

	float4x4 GetParentTransform()
	{
		return GetParentTranformRecursion(Hierarchy);
	}

public:
	AnimationAsset*									CurrentAsset = nullptr;

	std::vector<Sequence*>							Sequences;

	std::unordered_map<std::string, Montage>		Montages;
	std::unordered_map<std::string, BlendSpace>		BlendSpaces;
	std::unordered_map<std::string, Composite>		Composites;

	std::unordered_map<std::string, Morph>			Morphes;

	// �� ����޽� ������ ���ؽ� ����Ʈ�� ���� Weight�� ������ ����
	std::vector<std::vector<double>>	WeightAccumulation;

	inline AnimationAsset* GetAsset()
	{
		return CurrentAsset;
	}

	inline void SetAsset(AnimationAsset* newAsset)
	{
		CurrentAsset = newAsset;
	}

	inline Sequence* CreateSequence()
	{
		Sequences.push_back(new Sequence);
		return Sequences[Sequences.size() - 1];
	}

public:
	int BoneCount;
	Tree<Skeleton> BoneHierarchy;

	// ���� ��Ÿ�� ������Ʈ �Ѵ�.
	// 
	// ����� �״��� ���� ����.
	// ��ĳ����� ���� ���չ�Ģ�� �����ǹǷ� ��� ���ؽ��� ���ؼ� ���ο� ������ �� �ʿ䰡 ����.
	// ���� ȸ�� => ���� ����
	void UpdateBoneDelta();
	
	// ��Ÿ������ ����� �������� Ȯ��
	bool CheckSibilingSkeletal(ObjectData* target);
	// ������ �޽��� �������� Ȯ��
	bool CheckSibilingMesh(ObjectData* target);

	// ������ ����� �������� �ҷ���
	bool ImportAnimationSequence(std::string Path);
	// ������ ����� ��Ÿ�ָ� �ҷ���
	bool ImportAnimationMontage(std::string Path);
	// ������ ����� ���� �����̽��� �ҷ���
	bool ImportAnimationBlendSpace(std::string Path);
	// ������ ����� ������Ʈ�� �ҷ���
	bool ImportAnimationComposite(std::string Path);
	// ������ ����� ������ �ҷ���
	bool ImportAnimationMorph(std::string Path);

	// ������ �������� ����
	bool ExportAnimationSequence(ObjectData* target, std::string name = "");
	// ������ ��Ÿ�ָ� ����
	bool ExportAnimationMontage();
	// ������ ���� �����̽��� ����
	bool ExportAnimationBlendSpace();
	// ������ ������Ʈ�� ����
	bool ExportAnimationComposite();
	// ������ ������ ����
	bool ExportAnimationMorph(ObjectData* target, std::string name = "");

public:
	bool bIsEnableDepthStencil = true;
};

// path�� �� ���� ������ ���� �̸��� Ŭ����
std::string Path2FileName(const char* texIter);
// path�� ���� ���� ����
void CreateFolders(std::string path);
// ��Ʈ ���� ��� ���� �н��� ����
void GetAllSubFolders(std::string root, std::vector<std::string>& Folders);
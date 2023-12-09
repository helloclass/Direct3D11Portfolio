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

	// 부모 오브젝트에 탑제 시킨다
	bool AttachTo(ObjectData* Parent, std::string RootSocketName)
	{
		if (!Parent)
			return false;

		// 부모 뼈대 찾기
		ParentRootBoneName = RootSocketName;

		// 부모 하이어라키에 자식 오브젝트 트리 삽입
		Parent->Hierarchy.Insert(&Hierarchy);
		// 본 오브젝트의 루트로 Parent 트리 삽입
		Hierarchy.Parent = &Parent->Hierarchy;
		// 루트 == 부모 트리
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

	// 조상 부터 바로 윗 부모까지의 Transform을 모두 곱하여 리턴
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

	// 각 서브메쉬 단위의 버텍스 리스트에 대한 Weight를 가지고 있음
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

	// 본의 델타를 업데이트 한다.
	// 
	// 비용은 그다지 높지 않음.
	// 행렬끼리의 곱은 결합법칙이 성립되므로 모든 버텍스에 대해서 새로운 연산을 할 필요가 없음.
	// 연산 회수 => 뼈대 개수
	void UpdateBoneDelta();
	
	// 리타게팅할 뼈대와 동일한지 확인
	bool CheckSibilingSkeletal(ObjectData* target);
	// 모핑할 메쉬와 동일한지 확인
	bool CheckSibilingMesh(ObjectData* target);

	// 지정된 경로의 시퀸스를 불러옴
	bool ImportAnimationSequence(std::string Path);
	// 지정된 경로의 몽타주를 불러옴
	bool ImportAnimationMontage(std::string Path);
	// 지정된 경로의 블렌드 스페이스를 불러옴
	bool ImportAnimationBlendSpace(std::string Path);
	// 지정된 경로의 컴포지트를 불러옴
	bool ImportAnimationComposite(std::string Path);
	// 지정된 경로의 모프를 불러옴
	bool ImportAnimationMorph(std::string Path);

	// 본인의 시퀸스를 수출
	bool ExportAnimationSequence(ObjectData* target, std::string name = "");
	// 본인의 몽타주를 수출
	bool ExportAnimationMontage();
	// 본인의 블렌드 스페이스를 수출
	bool ExportAnimationBlendSpace();
	// 본인의 컴포지트를 수출
	bool ExportAnimationComposite();
	// 본인의 모프를 수출
	bool ExportAnimationMorph(ObjectData* target, std::string name = "");

public:
	bool bIsEnableDepthStencil = true;
};

// path의 맨 끝단 폴더나 파일 이름을 클립핑
std::string Path2FileName(const char* texIter);
// path에 대한 폴더 생성
void CreateFolders(std::string path);
// 루트 내의 모든 폴더 패스를 제공
void GetAllSubFolders(std::string root, std::vector<std::string>& Folders);
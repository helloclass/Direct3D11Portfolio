#pragma once
#include "ParticleObjectData.h"
#include "Memory.h"

// 피직스가 적용되지 않는 오브젝트 (UI 등)
class UIObjectData : public ObjectData
{
public:
	UIObjectData();

	void Awake() override;
	void Start() override;
	// Static은 위치가 변경 될 가능성 없음
	void Update() override;
	void Destroy() override;

public:
	std::unordered_map<std::string, Transform::transform*>::iterator Iterator;
	std::unordered_map<std::string, Transform::transform*>::iterator EndIterator;

	Tree<Skeleton>* ParentRootBone = nullptr;
	Tree<Skeleton>* ParentBindBone = nullptr;

public:
	// Max InstanceCount is 2000 
	void Instantiate() override;
	void Instantiate(UINT Number) override;
	void Instantiate(Instance& t) override;
	void Instantiate(std::string Name) override;
	void Instantiate(float3 Position) override;

	void Destroy(std::string name) override;
};

// 특수 피직스가 적용된 오브젝트
class ParticleObjectData : public ObjectData
{
public:
	ParticleObjectData();

	void Awake() override;
	void Start() override;
	// Static은 위치가 변경 될 가능성 없음
	void Update() override;
	void Destroy() override;

public:
	std::unordered_map<std::string, Transform::transform*>::iterator Iterator;
	std::unordered_map<std::string, Transform::transform*>::iterator EndIterator;

	Tree<Skeleton>* ParentRootBone = nullptr;
	Tree<Skeleton>* ParentBindBone = nullptr;

public:
	ParticlePhysics* PhysicsBody;

public:
	// Max InstanceCount is 2000 
	void Instantiate() override {}
	void Instantiate(UINT Number) override {}
	void Instantiate(Instance& t) override {}
	void Instantiate(std::string Name) override {}
	void Instantiate(float3 Position) override {}

	void Destroy(std::string name) override {}

};

class StaticObjectData : public ObjectData
{
public:
	StaticObjectData();

	void Awake() override;
	void Start() override;
	// Static은 위치가 변경 될 가능성 없음
	void Update() override;
	void Destroy() override;

public:
	std::unordered_map<std::string, Transform::transform*>::iterator Iterator;
	std::unordered_map<std::string, Transform::transform*>::iterator EndIterator;

	// 피직스 바디는 인스턴스당 하나씩 가지고 있어야 함.
	std::unordered_map<std::string, PxRigidStatic*> PhysicsBody;

	Tree<Skeleton>* ParentRootBone = nullptr;
	Tree<Skeleton>* ParentBindBone = nullptr;

public:
	// Max InstanceCount is 2000 
	void Instantiate() override;
	void Instantiate(UINT Number) override;
	void Instantiate(Instance& t) override;
	void Instantiate(std::string instName) override;
	void Instantiate(float3 Position) override;

	void Destroy(std::string name) override;

};

class KinematicObjectData : public ObjectData
{
public:
	KinematicObjectData();

	void Awake() override;
	// 인스턴스 만큼 PhysicsBody 채우기
	void Start() override;
	// Kinematic은 Dynamic과의 상호작용이 가능
	void Update() override;
	void Destroy() override;

public:
	std::unordered_map<std::string, Transform::transform*>::iterator Iterator;
	std::unordered_map<std::string, Transform::transform*>::iterator EndIterator;

	// 피직스 바디는 인스턴스당 하나씩 가지고 있어야 함.
	std::unordered_map<std::string, PxRigidDynamic*> PhysicsBody;

	Tree<Skeleton>* ParentRootBone = nullptr;
	Tree<Skeleton>* ParentBindBone = nullptr;

public:

	// Max InstanceCount is 2000 
	void Instantiate() override;
	void Instantiate(UINT Number) override;
	void Instantiate(Instance& t) override;
	void Instantiate(std::string instName) override;
	void Instantiate(float3 Position) override;

	void Destroy(std::string name) override;

};

class DynamicObjectData : public ObjectData
{
public:
	DynamicObjectData();

	void Awake() override;
	void Start() override;
	void Update() override;
	void Destroy() override;

public:
	std::unordered_map<std::string, Transform::transform*>::iterator Iterator;
	std::unordered_map<std::string, Transform::transform*>::iterator EndIterator;

	Tree<ObjectData*> Childs;
	// 해당 함수는 오직 Instance 0을 기준으로 하여 부모 인스턴스를 제공
	Instance getParentInstance();

public:
	// 피직스 바디는 인스턴스당 하나씩 가지고 있어야 함.
	std::unordered_map<std::string, PxRigidDynamic*> PhysicsBody;

	Tree<Skeleton>* ParentRootBone = nullptr;
	Tree<Skeleton>* ParentBindBone = nullptr;

	physx::PxTransform pxTransform;
	physx::PxVec3 pxVec3;

	// Get Info Physx to Instance
	float4 ParentPosition;
	float4 ParentRotation;

	float3 ResultPosition;
	float3 ResultRotation;

	fbxsdk::FbxVector4* vertices;
	fbxsdk::FbxVector4	result;

	fbxsdk::FbxAMatrix	matrix;
	float4x4			localMatrix;

	FbxSkin* lSkinDeformer;

	UINT lClusterCount = 0;
	UINT lClusterIndex = 0;
	FbxCluster* lCluster;

	int* mIndices = nullptr;

	UINT lSubmeshIDX = 0;

	Tree<Skeleton>* bone;

	fbxsdk::FbxVector4 BonePosition;
	fbxsdk::FbxVector4 BoneRotation;
	fbxsdk::FbxVector4 BoneScale;

	std::string BoneName;

public:
	// Max InstanceCount is 2000 
	void Instantiate() override;
	void Instantiate(UINT Number) override;
	void Instantiate(Instance& t) override;
	void Instantiate(std::string instName) override;
	void Instantiate(float3 Position) override;

	void Destroy(std::string name) override;

public:
	// 타겟의 모든 뼈대들을 리턴
	void GetAllBones(std::unordered_map<std::string, float4x4>& Bones);
	// 타겟의 모든 소켓들을 리턴
	void GetAllSockets(std::unordered_map<std::string, float4x4>& Socket);

};
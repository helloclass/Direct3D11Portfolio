#pragma once
#include "ParticleObjectData.h"
#include "Memory.h"

// �������� ������� �ʴ� ������Ʈ (UI ��)
class UIObjectData : public ObjectData
{
public:
	UIObjectData();

	void Awake() override;
	void Start() override;
	// Static�� ��ġ�� ���� �� ���ɼ� ����
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

// Ư�� �������� ����� ������Ʈ
class ParticleObjectData : public ObjectData
{
public:
	ParticleObjectData();

	void Awake() override;
	void Start() override;
	// Static�� ��ġ�� ���� �� ���ɼ� ����
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
	// Static�� ��ġ�� ���� �� ���ɼ� ����
	void Update() override;
	void Destroy() override;

public:
	std::unordered_map<std::string, Transform::transform*>::iterator Iterator;
	std::unordered_map<std::string, Transform::transform*>::iterator EndIterator;

	// ������ �ٵ�� �ν��Ͻ��� �ϳ��� ������ �־�� ��.
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
	// �ν��Ͻ� ��ŭ PhysicsBody ä���
	void Start() override;
	// Kinematic�� Dynamic���� ��ȣ�ۿ��� ����
	void Update() override;
	void Destroy() override;

public:
	std::unordered_map<std::string, Transform::transform*>::iterator Iterator;
	std::unordered_map<std::string, Transform::transform*>::iterator EndIterator;

	// ������ �ٵ�� �ν��Ͻ��� �ϳ��� ������ �־�� ��.
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
	// �ش� �Լ��� ���� Instance 0�� �������� �Ͽ� �θ� �ν��Ͻ��� ����
	Instance getParentInstance();

public:
	// ������ �ٵ�� �ν��Ͻ��� �ϳ��� ������ �־�� ��.
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
	// Ÿ���� ��� ������� ����
	void GetAllBones(std::unordered_map<std::string, float4x4>& Bones);
	// Ÿ���� ��� ���ϵ��� ����
	void GetAllSockets(std::unordered_map<std::string, float4x4>& Socket);

};
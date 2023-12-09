#include "PhysicsObjectData.h"

/*

	Physics Data Update 규정

	1. Play 상태가 아닌 경우 Transform -> Physx로만 데이터를 넘겨 줄 수 있다.
	2. Play 상태에서는 Physx -> Transform으로만 데이터를 넘겨 줄 수 있다.

*/

#pragma region Transpose

UIObjectData::UIObjectData() {
	ObjectData::ObjectData();

	physicsType = PhysicsType::NONE_TYPE;
}

void UIObjectData::Awake() {
	ObjectData::Awake();

}

void UIObjectData::Start() {
	ObjectData::Start();

}

void UIObjectData::Update() {
	ObjectData::Update();

	Iterator	= transform->InstanceBuffer.begin();
	EndIterator = transform->InstanceBuffer.end();

	// Get Info Physx to Instance
	float3 Position;

	if (Hierarchy.Parent)
	{
		ParentRootBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentRootBoneName);
		ParentBindBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentBindBoneName);
	}
	
	while (Iterator != EndIterator)
	{
		// 부모 오브젝트가 존재할 시
		if (Iterator->second && ParentRootBone)
		{
			// 현 Instance의 Transform을 
			Iterator->second->GetLocalPosition(Position);
			// 부모 (글로벌) Transform에 적용시킨다.
			Position = Position * ParentRootBone->Item.TranslationMatrix;
			if (ParentBindBone)
			{
				Position = Position * ParentBindBone->Item.TranslationMatrix;
			}
			Iterator->second->SetPosition(Position);
		}

		// Not Play Mode
		if (!PhysxPool::getInstance().IsPlay)
		{
			// Get Info Instance to Physx
			if (Iterator->second && Iterator->second->bIsDirtyInstance)
			{
				Iterator->second->bIsDirtyInstance = false;
			}
		}
		// Play Mode
		else
		{

		}

		Iterator++;

	}

}

void UIObjectData::Destroy() {
	ObjectData::Destroy();
}

ParticleObjectData::ParticleObjectData() {
	ObjectData::ObjectData();

	physicsType = PhysicsType::PARTICLE_TYPE;
}

void ParticleObjectData::Awake() {
	ObjectData::Awake();

}

void ParticleObjectData::Start() {
	ObjectData::Start();

	std::vector<unsigned> ParticleNum;

	for (Tree<ObjectData*>* child : Hierarchy.Childs)
	{
		ParticleNum.push_back(child->Item->transform->InstanceCount);
	}

	PhysicsBody = new ParticlePhysics(ParticleNum);

}

void ParticleObjectData::Update() {
	ObjectData::Update();

	PhysicsBody->ParticleUpdate();

#pragma region ParticleUpdate

	int ChildIDX = 0;
	int InstanceIDX = 0;
	for (Tree<ObjectData*>* child : Hierarchy.Childs)
	{
		InstanceIDX = 0;
		PhysicsBody->ThreadPool[ChildIDX]->ParticlePositionBuffer;

		for (std::pair<std::string, Transform::transform*> inst : child->Item->transform->InstanceBuffer)
		{
			inst.second->SetGlobalPosition(
				PhysicsBody->ThreadPool[ChildIDX]->ParticlePositionBuffer[InstanceIDX]
			);

			InstanceIDX++;
		}

		ChildIDX++;
	}

#pragma endregion

	Iterator = transform->InstanceBuffer.begin();
	EndIterator = transform->InstanceBuffer.end();

	// Get Info Physx to Instance
	float3 Position;

	// Find Parent, Bind Socket
	if (Hierarchy.Parent)
	{
		ParentRootBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentRootBoneName);
		ParentBindBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentBindBoneName);
	}

	while (Iterator != EndIterator)
	{
		// 부모 오브젝트가 존재할 시
		if (Iterator->second && ParentRootBone)
		{
			// 현 Instance의 Transform을 
			Iterator->second->GetLocalPosition(Position);
			// 부모 (글로벌) Transform에 적용시킨다.
			Position = Position * ParentRootBone->Item.TranslationMatrix;
			if (ParentBindBone)
			{
				Position = Position * ParentBindBone->Item.TranslationMatrix;
			}
			Iterator->second->SetPosition(Position);
		}

		// Not Play Mode
		if (!PhysxPool::getInstance().IsPlay)
		{
			// Get Info Instance to Physx
			if (Iterator->second && Iterator->second->bIsDirtyInstance)
			{
				Iterator->second->bIsDirtyInstance = false;
			}
		}
		// Play Mode
		else
		{

		}

		Iterator++;

	}
}

void ParticleObjectData::Destroy() {
	ObjectData::Destroy();

	for (std::pair<std::string, Transform::transform*> t : transform->InstanceBuffer)
	{
		delete t.second;
	}

	transform->InstanceCount = 0;
	transform->InstanceDirty = true;

	delete PhysicsBody;
}

StaticObjectData::StaticObjectData() {
	ObjectData::ObjectData();

	physicsType = PhysicsType::STATIC_TYPE;
}

void StaticObjectData::Awake() {
	ObjectData::Awake();

}

void StaticObjectData::Start() {
	ObjectData::Start();

	Iterator = transform->InstanceBuffer.begin();
	EndIterator = transform->InstanceBuffer.end();

	PxShape* shape = nullptr;
	PxRigidStatic* body = nullptr;
	PxTransform position;

	// 인스턴스 만큼 PhysicsBody 채우기
	float3 NewBoxScale;
	while (Iterator != EndIterator)
	{
		// Collider Box 생성
		Iterator->second->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);

		// 바디 만들고
		body = 
			PhysxPool::getInstance().data.CreateStatic(
				PxTransform(PxVec3(0, 0, 0)),
				shape
			);

		// 인스턴스 위치로 변경
		Iterator->second->GetLocalTransform(position);
		body->setGlobalPose(position);

		// PhysicsBody 추가
		PhysicsBody[Iterator->first] = body;

		Iterator++;
	}
}

void StaticObjectData::Update() {
	ObjectData::Update();

	Iterator	= transform->InstanceBuffer.begin();
	EndIterator = transform->InstanceBuffer.end();

	physx::PxTransform buffer = { 0.0f, 0.0f, 0.0f };

	// Get Info Physx to Instance
	float3 Position;

	if (Hierarchy.Parent)
	{
		ParentRootBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentRootBoneName);
		ParentBindBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentBindBoneName);
	}

	while (Iterator != EndIterator)
	{
		if (Iterator->second && ParentRootBone)
		{
			Iterator->second->GetLocalPosition(Position);

			Position = Position * ParentRootBone->Item.TranslationMatrix;
			if (ParentBindBone)
			{
				Position = Position * ParentBindBone->Item.TranslationMatrix;
			}
			Iterator->second->SetPosition(Position);
		}

		// Not Play Mode
		if (!PhysxPool::getInstance().IsPlay)
		{
			if (Iterator->second->bIsDirtyInstance)
			{
				// Set Position, Rotation, Scale
				Iterator->second->GetLocalTransform(buffer);
				PhysicsBody[Iterator->first]->setGlobalPose(buffer);

				// Static Can't Setting velocity, torque, accumulation

				Iterator->second->bIsDirtyInstance = false;
			}
		}
		// Play Mode
		else
		{

		}
		
		// Static Can't Getting velocity, torque, accumulation

		Iterator++;
	}
}

void StaticObjectData::Destroy() {
	ObjectData::Destroy();
}

KinematicObjectData::KinematicObjectData() {
	ObjectData::ObjectData();

	physicsType = PhysicsType::KINEMATIC_TYPE;
}

void KinematicObjectData::Awake() {
	ObjectData::Awake();

}

void KinematicObjectData::Start()
{
	ObjectData::Start();

	Iterator = transform->InstanceBuffer.begin();
	EndIterator = transform->InstanceBuffer.end();

	PxRigidDynamic* body = nullptr;
	PxShape* shape = nullptr;
	PxTransform position;
	float density = 0.1f;
	PxVec3 velocity = {0.0f, 0.0f, 0.0f};

	float3 NewBoxScale;

	while (Iterator != EndIterator)
	{
		Iterator->second->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);

		body = PhysxPool::getInstance().data.CreateKinematic(
			PxTransform(PxVec3(0, 0, 0)),
			shape,
			density,
			velocity
		);

		Iterator->second->GetLocalTransform(position);
		body->setGlobalPose(position);

		PhysicsBody[Iterator->first] = body;

		Iterator++;
	}
}

void KinematicObjectData::Update() 
{
	ObjectData::Update();

	Iterator	= transform->InstanceBuffer.begin();
	EndIterator = transform->InstanceBuffer.end();

	physx::PxTransform pxTransform;
	physx::PxVec3 pxVec3;

	// Get Info Physx to Instance
	float3 Position;

	if (Hierarchy.Parent)
	{
		ParentRootBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentRootBoneName);
		ParentBindBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentBindBoneName);
	}

	while (Iterator != EndIterator)
	{
		if (Iterator->second && ParentRootBone)
		{
			Iterator->second->GetLocalPosition(Position);

			Position = Position * ParentRootBone->Item.TranslationMatrix;
			if (ParentBindBone)
			{
				Position = Position * ParentBindBone->Item.TranslationMatrix;
			}
			Iterator->second->SetPosition(Position);
		}

		// Not Play Mode
		if (!PhysxPool::getInstance().IsPlay)
		{
			// Get Info Instance to Physx
			if (Iterator->second->bIsDirtyInstance)
			{
				// Set Position, Rotation, Scale
				Iterator->second->GetLocalTransform(pxTransform);
				PhysicsBody[Iterator->first]->setGlobalPose(pxTransform);

				pxVec3.x = Iterator->second->Velocity->x;
				pxVec3.y = Iterator->second->Velocity->y;
				pxVec3.z = Iterator->second->Velocity->z;

				PhysicsBody[Iterator->first]->setLinearVelocity(pxVec3);

				pxVec3.x = Iterator->second->Torque->x;
				pxVec3.y = Iterator->second->Torque->y;
				pxVec3.z = Iterator->second->Torque->z;

				PhysicsBody[Iterator->first]->setAngularVelocity(pxVec3);

				Iterator->second->bIsDirtyInstance = false;
			}
		}
		// Play Mode
		else
		{
			pxTransform = PhysicsBody[Iterator->first]->getGlobalPose();
			Iterator->second->SetTransform(pxTransform, false);

			pxVec3 = PhysicsBody[Iterator->first]->getLinearVelocity();
			Iterator->second->SetVelocity(pxVec3, false);

			pxVec3 = PhysicsBody[Iterator->first]->getAngularVelocity();
			Iterator->second->SetTorque(pxVec3, false);
		}

		Iterator++;
	}
}

void KinematicObjectData::Destroy() {
	ObjectData::Destroy();
}

DynamicObjectData::DynamicObjectData()
{
	ObjectData::ObjectData();

	physicsType = PhysicsType::DYNAMIC_TYPE;

}

void DynamicObjectData::Awake() {
	ObjectData::Awake();

}

void DynamicObjectData::Start()
{
	Iterator = transform->InstanceBuffer.begin();
	EndIterator = transform->InstanceBuffer.end();

	PxRigidDynamic* body = nullptr;
	PxShape* shape = nullptr;
	PxTransform position;
	float density = 0.1f;
	PxVec3 velocity = {0.0f, 0.0f, 0.0f};

	float3 NewBoxScale;

	while (Iterator != EndIterator)
	{
		Iterator->second->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		shape = PhysxPool::getInstance().data.CreateBox(
			NewBoxScale
		);

		body = PhysxPool::getInstance().data.CreateDynamic(
			PxTransform(PxVec3(0, 0, 0)),
			shape->getGeometry().any(),
			velocity
		);

		Iterator->second->GetLocalTransform(position);
		body->setGlobalPose(position);

		PhysicsBody[Iterator->first] = body;

		Iterator++;
	}
}

void DynamicObjectData::Update() 
{
	ObjectData::Update();

	/*
		애니메이션 편집 기술 델타를 업데이트 시킴.

		델타란 기존 애니메이션에 대한 변형 매트릭스에 추가적으로 입력되는 가중치로
		기존 변형에서 편집 된 변형으로 만들어주는 가중치 역할을 함.

		O(n) 복잡도로 비용이 크게 비싸지는 않음.
	*/
	//UpdateBoneDelta();

	Iterator = transform->InstanceBuffer.begin();
	EndIterator = transform->InstanceBuffer.end();

#pragma region Update Bone Transpose

	vertices = meshFBXDatas[lSubmeshIDX]->GetControlPoints();
	lSkinDeformer = (FbxSkin*)meshFBXDatas[lSubmeshIDX]->GetDeformer(0, FbxDeformer::eSkin);
	lClusterCount = lSkinDeformer->GetClusterCount();

	for (lClusterIndex = 0; lClusterIndex < lClusterCount; lClusterIndex++)
	{
		lCluster = lSkinDeformer->GetCluster(lClusterIndex);
		mIndices = lCluster->GetControlPointIndices();

		if (!mIndices)
			continue;

		BoneName = lCluster->GetNameOnly();

		bone = BoneHierarchy.Find(std::string(lCluster->GetNameOnly()));

		if (!GetAsset())
			continue;

		GetAsset()->GetAnimationFBXMatrix(lSubmeshIDX, lClusterIndex, matrix);

		BonePosition = matrix.MultT(vertices[mIndices[0]]);
	 	BoneRotation = matrix.MultR(vertices[mIndices[0]]);

		bone->Item.Position.x = BonePosition.mData[0];
		bone->Item.Position.y = BonePosition.mData[1];
		bone->Item.Position.z = BonePosition.mData[2];
		bone->Item.Position.w = 1.0f;

		bone->Item.Rotation.x = BoneRotation.mData[0];
		bone->Item.Rotation.y = BoneRotation.mData[1];
		bone->Item.Rotation.z = BoneRotation.mData[2];
		bone->Item.Rotation.w = 1.0f;

		bone->Item.Scale = { 1.0f, 1.0f, 1.0f };
	}

#pragma endregion

	if (Hierarchy.Parent)
	{
		ParentRootBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentRootBoneName);
		ParentBindBone = Hierarchy.Parent->Item->BoneHierarchy.Find(ParentBindBoneName);
	}


	float4x4 FinalTransform;
	fbxsdk::FbxVector4 FinalPosition; 
	fbxsdk::FbxVector4 FinalRotation;
	fbxsdk::FbxQuaternion FinalQuaternion;

	while (Iterator != EndIterator)
	{
		if (Iterator->second)
		{
			if (ParentRootBone)
			{
				ParentPosition = ParentRootBone->Item.Position;
				ParentRotation = ParentRootBone->Item.Rotation;

				//if (ParentBindBone)
				//{
				//	ParentPosition =
				//		ParentPosition + ParentBindBone->Item.Position;
				//	ParentRotation =
				//		ParentRotation + ParentBindBone->Item.Rotation;
				//}

				ClampVector4(ParentRotation, 0.0f, 360.0f);

				ResultPosition = ParentPosition.xyz;
				ResultRotation = ParentRotation.xyz;

				// Set Global
				Iterator->second->SetGlobalPosition(ResultPosition);
				Iterator->second->SetGlobalRotation(ResultRotation);
				Iterator->second->SetGlobalScale(1.0f, 1.0f, 1.0f);

			}

			Iterator->second->bIsDirtyInstance = true;
		}

		// Not Play Mode
		if (!PhysxPool::getInstance().IsPlay)
		{
			// Get Info Instance to Physx
			if (Iterator->second->bIsDirtyInstance)
			{
				// Calc Final Transform (Global + Local)
				Iterator->second->GetTransform(pxTransform);
				PhysicsBody[Iterator->first]->setGlobalPose(pxTransform);

				Iterator->second->GetPosition(
					Iterator->second->Matrix->Position[0],
					Iterator->second->Matrix->Position[1],
					Iterator->second->Matrix->Position[2]
				);
				Iterator->second->GetRotation(
					Iterator->second->Matrix->Rotation[0],
					Iterator->second->Matrix->Rotation[1],
					Iterator->second->Matrix->Rotation[2]
				);
				Iterator->second->Matrix->Scale[0]		= 1.0f;
				Iterator->second->Matrix->Scale[1]		= 1.0f;
				Iterator->second->Matrix->Scale[2]		= 1.0f;

				pxVec3.x = Iterator->second->Velocity->x;
				pxVec3.y = Iterator->second->Velocity->y;
				pxVec3.z = Iterator->second->Velocity->z;

				PhysicsBody[Iterator->first]->setLinearVelocity(pxVec3);

				pxVec3.x = Iterator->second->Torque->x;
				pxVec3.y = Iterator->second->Torque->y;
				pxVec3.z = Iterator->second->Torque->z;

				PhysicsBody[Iterator->first]->setAngularVelocity(pxVec3);

				Iterator->second->bIsDirtyInstance = false;
			}
		}
		// Play Mode
		else 
		{
			pxTransform = PhysicsBody[Iterator->first]->getGlobalPose();
			Iterator->second->SetTransform(pxTransform, false);

			pxVec3 = PhysicsBody[Iterator->first]->getLinearVelocity();
			Iterator->second->SetVelocity(pxVec3, false);

			pxVec3 = PhysicsBody[Iterator->first]->getAngularVelocity();
			Iterator->second->SetTorque(pxVec3, false);
		}

		Iterator++;
	}
}

void DynamicObjectData::Destroy() {
	ObjectData::Destroy();
}

Instance DynamicObjectData::getParentInstance()
{
	Instance ParentInstance = {};

	Tree<ObjectData*>* Root = &Childs;
	std::string Name;

	while (Root->Parent)
	{
		Root = Root->Parent;

		Name = Root->Item->transform->InstanceBuffer.begin()->first;

		ParentInstance.Position[0] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Position[0];
		ParentInstance.Position[1] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Position[1];
		ParentInstance.Position[2] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Position[2];

		ParentInstance.Rotation[0] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Rotation[0];
		ParentInstance.Rotation[1] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Rotation[1];
		ParentInstance.Rotation[2] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Rotation[2];

		ParentInstance.Scale[0] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Scale[0];
		ParentInstance.Scale[1] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Scale[1];
		ParentInstance.Scale[2] += Root->Item->transform->InstanceBuffer[Name]->LocalMatrix->Scale[2];
	}

	return ParentInstance;
}

#pragma region Implementation Instantiate

	// Max InstanceCount is 2000 
	void UIObjectData::Instantiate()
	{
		std::string instName = transform->Name + "_" + std::to_string(transform->ID_Count);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform(
			transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0
		);

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	// Max InstanceCount is 2000 
	void UIObjectData::Instantiate(UINT Number)
	{
		UINT iter;
		std::string instName;

		for (iter = 0; iter < Number; iter++)
		{
			instName = transform->Name + "_" + std::to_string(transform->ID_Count);

			transform->InstanceBuffer.insert(
				std::pair<std::string, Transform::transform*>(
					instName,
					new Transform::transform()
					)
			);

			transform->InstanceBuffer[instName]->SetTransform(
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0
			);

			transform->ID_Count++;
			transform->InstanceCount++;
		}

		transform->InstanceDirty = true;
	}

	void UIObjectData::Instantiate(Instance& t)
	{
		std::string instName = transform->Name + "(" + std::to_string(transform->ID_Count) + ")";

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform(
			t.Position[0], t.Position[1], t.Position[2],
			t.Rotation[0], t.Rotation[1], t.Rotation[2],
			t.Scale[0], t.Scale[0], t.Scale[0]
		);

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void UIObjectData::Instantiate(std::string instName)
	{
		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform(
			transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f
		);

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void UIObjectData::Instantiate(float3 Position)
	{
		std::string instName = transform->Name + "(" + std::to_string(transform->ID_Count) + ")";

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform(
			Position.x, Position.y, Position.z,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f
		);

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void UIObjectData::Destroy(std::string name)
	{
		for (std::pair<std::string, Transform::transform*> t : transform->InstanceBuffer)
		{
			if (!strcmp(t.first.c_str(), name.c_str()))
			{
				delete t.second;

				transform->InstanceCount--;
				transform->InstanceDirty = true;

				break;
			}
		}
	}

	// Max InstanceCount is 2000 
	void StaticObjectData::Instantiate()
	{
		std::string instName = transform->Name + "_" + std::to_string(transform->ID_Count);

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);
		
		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform(
			transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f
		);

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxRigidStatic* body = PhysxPool::getInstance().data.CreateStatic(
			PxTransform(PxVec3(0, 0, 0)),
			shape
		);

		// 인스턴스 위치로 변경
		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);
		body->setGlobalPose(position);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void StaticObjectData::Instantiate(UINT Number)
	{
		std::string instName;
		UI_ListBox* listbox;

		PxShape* shape;
		PxRigidStatic* body;

		PxTransform position;

		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		UINT iter;
		for (iter = 0; iter < Number; iter++)
		{
			instName = transform->Name + "_" + std::to_string(transform->ID_Count);

			listbox = &HWNDPool::getInstance().WorldListBox;
			listbox->AppendListBoxMessage(instName);

			transform->InstanceBuffer.insert(
				std::pair<std::string, Transform::transform*>(
					instName,
					new Transform::transform()
					)
			);

			transform->InstanceBuffer[instName]->SetTransform(
				transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
			);

			shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
			body = PhysxPool::getInstance().data.CreateStatic(
				PxTransform(PxVec3(0, 0, 0)),
				shape
			);

			// 인스턴스 위치로 변경
			transform->InstanceBuffer[instName]->GetLocalTransform(position);
			body->setGlobalPose(position);

			// PhysicsBody 추가
			PhysicsBody[instName] = body;

			transform->ID_Count++;
			transform->InstanceCount++;
		}

		transform->InstanceDirty = true;

	}
	void StaticObjectData::Instantiate(Instance& t)
	{
		std::string instName = transform->Name + "(" + std::to_string(transform->ID_Count) + ")";

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				t.Position[0],	t.Position[1],	t.Position[2],
				t.Rotation[0],	t.Rotation[1],	t.Rotation[2],
				t.Scale[0],		t.Scale[0],		t.Scale[0]
		);

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxRigidStatic* body = PhysxPool::getInstance().data.CreateStatic(
			PxTransform(PxVec3(0)),
			shape
		);

		// 인스턴스 위치로 변경
		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);
		body->setGlobalPose(position);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void StaticObjectData::Instantiate(std::string instName)
	{
		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
		);

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxRigidStatic* body = PhysxPool::getInstance().data.CreateStatic(
			PxTransform(PxVec3(0, 0, 0)),
			shape
		);

		// 인스턴스 위치로 변경
		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);
		body->setGlobalPose(position);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void StaticObjectData::Instantiate(float3 Position)
	{
		std::string instName = transform->Name + "(" + std::to_string(transform->ID_Count) + ")";

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				Position.x, Position.y, Position.z,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
		);

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxRigidStatic* body = PhysxPool::getInstance().data.CreateStatic(
			PxTransform(PxVec3(0)),
			shape
		);
		// 인스턴스 위치로 변경
		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);
		body->setGlobalPose(position);
		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void StaticObjectData::Destroy(std::string name)
	{
		for (std::pair<std::string, Transform::transform*> t : transform->InstanceBuffer)
		{
			if (!strcmp(t.first.c_str(), name.c_str()))
			{
				delete t.second;

				// World List 에서 제거하기
				transform->InstanceBuffer.erase(t.first);
				// Physx Body 제거하기
				PhysicsBody.erase(t.first.c_str());

				transform->InstanceCount--;
				transform->InstanceDirty = true;

				break;
			}
		}
	}


	// Max InstanceCount is 2000 
	void KinematicObjectData::Instantiate()
	{
		std::string instName = transform->Name + "_" + std::to_string(transform->ID_Count);

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
		);

		float density = 0.1f;
		PxVec3 velocity = {0.0f, 0.0f, 0.0f};

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxRigidDynamic* body = PhysxPool::getInstance().data.CreateKinematic(
			PxTransform(PxVec3(0)),
			shape,
			density,
			velocity
		);

		// 인스턴스 위치로 변경
		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);
		body->setGlobalPose(position);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	// Max InstanceCount is 2000 
	void KinematicObjectData::Instantiate(UINT Number)
	{
		std::string instName;
		UI_ListBox* listbox;

		float density = 0.1f;
		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape;
		PxRigidDynamic* body;

		PxTransform position;

		for (UINT iter = 0; iter < Number; iter++)
		{
			instName = transform->Name + "_" + std::to_string(transform->ID_Count);

			listbox = &HWNDPool::getInstance().WorldListBox;
			listbox->AppendListBoxMessage(instName);

			transform->InstanceBuffer.insert(
				std::pair<std::string, Transform::transform*>(
					instName,
					new Transform::transform()
					)
			);

			transform->InstanceBuffer[instName]->SetTransform(
				transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
			);

			shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
			body = PhysxPool::getInstance().data.CreateKinematic(
				PxTransform(PxVec3(0)),
				shape,
				density,
				velocity
			);

			// 인스턴스 위치로 변경
			transform->InstanceBuffer[instName]->GetLocalTransform(position);
			body->setGlobalPose(position);

			// PhysicsBody 추가
			PhysicsBody[instName] = body;

			transform->ID_Count++;
			transform->InstanceCount++;
		}

		transform->InstanceDirty = true;
	}

	void KinematicObjectData::Instantiate(Instance& t)
	{
		std::string instName = transform->Name + "(" + std::to_string(transform->ID_Count) + ")";

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				t.Position[0],	t.Position[1],	t.Position[2],
				t.Rotation[0],	t.Rotation[1],	t.Rotation[2],
				t.Scale[0],		t.Scale[0],		t.Scale[0]
		);

		float density = 0.1f;
		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxRigidDynamic* body = PhysxPool::getInstance().data.CreateKinematic(
			PxTransform(PxVec3(0)),
			shape,
			density,
			velocity
		);

		// 인스턴스 위치로 변경
		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);
		body->setGlobalPose(position);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void KinematicObjectData::Instantiate(std::string instName)
	{
		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform(
				transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
		);

		float density = 0.1f;
		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxRigidDynamic* body = PhysxPool::getInstance().data.CreateKinematic(
			PxTransform(PxVec3(0)),
			shape,
			density,
			velocity
		);

		// 인스턴스 위치로 변경
		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);
		body->setGlobalPose(position);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void KinematicObjectData::Instantiate(float3 Position)
	{
		std::string instName = transform->Name + "(" + std::to_string(transform->ID_Count) + ")";

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				Position.x, Position.y, Position.z,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
		);

		float density = 0.1f;
		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		// 바디 만들고
		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxRigidDynamic* body = PhysxPool::getInstance().data.CreateKinematic(
			PxTransform(PxVec3(0)),
			shape,
			density,
			velocity
		);
		// 인스턴스 위치로 변경
		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);
		body->setGlobalPose(position);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void KinematicObjectData::Destroy(std::string name)
	{
		for (std::pair<std::string, Transform::transform*> t : transform->InstanceBuffer)
		{
			if (!strcmp(t.first.c_str(), name.c_str()))
			{
				delete t.second;

				// World List 에서 제거하기
				transform->InstanceBuffer.erase(t.first);
				// Physx Body 제거하기
				PhysicsBody.erase(t.first.c_str());

				transform->InstanceCount--;
				transform->InstanceDirty = true;

				break;
			}
		}
	}


	// Max InstanceCount is 2000 
	void DynamicObjectData::Instantiate()
	{
		std::string instName = transform->Name + "_" + std::to_string(transform->ID_Count);

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform(
				transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
		);

		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);
		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		// 바디 만들기
		PxRigidDynamic* body = PhysxPool::getInstance().data.CreateDynamic(
			PxTransform(PxVec3(0, 0, 0)),
			shape->getGeometry().any(),
			velocity
		);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	// Max InstanceCount is 2000 
	void DynamicObjectData::Instantiate(UINT Number)
	{
		std::string instName;
		UI_ListBox* listbox;

		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxShape* shape;
		PxRigidDynamic* body;

		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		for (UINT iter = 0; iter < Number; iter++)
		{
			instName = transform->Name + "_" + std::to_string(transform->ID_Count);

			listbox = &HWNDPool::getInstance().WorldListBox;
			listbox->AppendListBoxMessage(instName);

			transform->InstanceBuffer.insert(
				std::pair<std::string, Transform::transform*>(
					instName,
					new Transform::transform()
					)
			);

			transform->InstanceBuffer[instName]->SetTransform(
				transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
			);

			shape = PhysxPool::getInstance().data.CreateBox(NewBoxScale);

			// 바디 만들기
			body = PhysxPool::getInstance().data.CreateDynamic(
				PxTransform(PxVec3(0, 0, 0)),
				shape->getGeometry().any(),
				velocity
			);

			// PhysicsBody 추가
			PhysicsBody[instName] = body;

			transform->ID_Count++;
			transform->InstanceCount++;
		}

		transform->InstanceDirty = true;
	}

	void DynamicObjectData::Instantiate(Instance& t)
	{
		std::string instName = transform->Name + "(" + std::to_string(transform->ID_Count) + ")";

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				t.Position[0],	t.Position[1],	t.Position[2],
				t.Rotation[0],	t.Rotation[1],	t.Rotation[2],
				t.Scale[0],		t.Scale[0],		t.Scale[0]
		);

		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);

		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxGeometry geom = PhysxPool::getInstance().data.CreateBox(NewBoxScale)->getGeometry().any();
		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		// 바디 만들기
		PxRigidDynamic* body = PhysxPool::getInstance().data.CreateDynamic(
			position,
			geom,
			velocity
		);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void DynamicObjectData::Instantiate(std::string instName)
	{
		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				transform->InstanceBuffer.size() * 25.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
		);

		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);

		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxGeometry geom = PhysxPool::getInstance().data.CreateBox(NewBoxScale)->getGeometry().any();
		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		// 바디 만들기
		PxRigidDynamic* body = PhysxPool::getInstance().data.CreateDynamic(
			position,
			geom,
			velocity
		);
		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void DynamicObjectData::Instantiate(float3 Position)
	{
		std::string instName = transform->Name + "(" + std::to_string(transform->ID_Count) + ")";

		UI_ListBox* listbox = &HWNDPool::getInstance().WorldListBox;
		listbox->AppendListBoxMessage(instName);

		transform->InstanceBuffer.insert(
			std::pair<std::string, Transform::transform*>(
				instName,
				new Transform::transform()
			)
		);

		transform->InstanceBuffer[instName]->SetTransform (
				Position.x, Position.y, Position.z,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f
		);

		PxTransform position;
		transform->InstanceBuffer[instName]->GetLocalTransform(position);

		float3 NewBoxScale;
		transform->InstanceBuffer[instName]->GetLocalScale(NewBoxScale);
		NewBoxScale = NewBoxScale * 0.5f;

		PxGeometry geom = PhysxPool::getInstance().data.CreateBox(NewBoxScale)->getGeometry().any();
		PxVec3 velocity = { 0.0f, 0.0f, 0.0f };

		// 바디 만들기
		PxRigidDynamic* body = PhysxPool::getInstance().data.CreateDynamic(
			position,
			geom,
			velocity
		);

		// PhysicsBody 추가
		PhysicsBody[instName] = body;

		transform->ID_Count++;
		transform->InstanceCount++;
		transform->InstanceDirty = true;
	}

	void DynamicObjectData::Destroy(std::string name)
	{
		for (std::pair<std::string, Transform::transform*> t : transform->InstanceBuffer)
		{
			if (!strcmp(t.first.c_str(), name.c_str()))
			{
				delete t.second;

				// World List 에서 제거하기
				transform->InstanceBuffer.erase(t.first);
				// Physx Body 제거하기
				PhysicsBody.erase(t.first.c_str());
			
				transform->InstanceCount--;
				transform->InstanceDirty = true;

				break;
			}
		}
	}


	void DynamicObjectData::GetAllBones(std::unordered_map<std::string, float4x4>& Bones)
	{
		UINT submeshIDX = 0;
		UINT clusterIndex = 0;

		int currentFrame = 0;

		FbxSkin* lSkinDeformer;

		UINT lClusterCount = 0;
		FbxCluster* lCluster;

		std::string name;
		float4x4 translate;
		int i(0), j(0), k(0);

		fbxsdk::FbxVector4* vertices;
		fbxsdk::FbxVector4 result;

		fbxsdk::FbxAMatrix matrix;

		int* mIndices = nullptr;

		// 애니메이션을 업데이트 한다.
		if (objType == ObjectType::FBX_SKINNED_TYPE)
		{
			if (CurrentAsset->mType == AnimationAsset::AnimationAssetType::SEQUENCE)
			{
				/*Sequence* asset = (Sequence*)GetAsset();*/
				Sequence* asset = reinterpret_cast<Sequence*>(GetAsset());

				currentFrame = asset->mFrame;

				for (submeshIDX = 0; submeshIDX < SubmeshCount; submeshIDX++)
				{
					vertices = meshFBXDatas[submeshIDX]->GetControlPoints();

					lSkinDeformer =
						(FbxSkin*)meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

					lClusterCount =
						lSkinDeformer->GetClusterCount();

					for (clusterIndex = 0; clusterIndex < lClusterCount; clusterIndex++)
					{
						lCluster = lSkinDeformer->GetCluster(clusterIndex);
						mIndices = lCluster->GetControlPointIndices();

						if (!mIndices)
							continue;

						name = lCluster->GetNameOnly();

						asset->GetAnimationMatrix(submeshIDX, clusterIndex, translate);

						result = matrix.MultT(vertices[mIndices[0]]);

						Bones[name] = translate;

					} // ClusterCount
				} // SubmeshCount
			}
			else if (CurrentAsset->mType == AnimationAsset::AnimationAssetType::MONTAGE)
			{
				Montage* asset = (Montage*)GetAsset();

				currentFrame = asset->mFrame;

				for (submeshIDX = 0; submeshIDX < SubmeshCount; submeshIDX++)
				{
					vertices = meshFBXDatas[submeshIDX]->GetControlPoints();

					lSkinDeformer =
						(FbxSkin*)meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

					lClusterCount =
						lSkinDeformer->GetClusterCount();

					for (clusterIndex = 0; clusterIndex < lClusterCount; clusterIndex++)
					{
						lCluster = lSkinDeformer->GetCluster(clusterIndex);
						mIndices = lCluster->GetControlPointIndices();

						if (!mIndices)
							continue;

						name = lCluster->GetNameOnly();

						asset->GetAnimationMatrix(submeshIDX, clusterIndex, translate);

						result = matrix.MultT(vertices[mIndices[0]]);

						Bones[name] = translate;

					} // ClusterCount
				} // SubmeshCount
			}
			else
			{
				BlendSpace* asset = (BlendSpace*)GetAsset();

				currentFrame = asset->mFrame;

				for (submeshIDX = 0; submeshIDX < SubmeshCount; submeshIDX++)
				{
					vertices = meshFBXDatas[submeshIDX]->GetControlPoints();

					lSkinDeformer =
						(FbxSkin*)meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

					lClusterCount =
						lSkinDeformer->GetClusterCount();

					for (clusterIndex = 0; clusterIndex < lClusterCount; clusterIndex++)
					{
						lCluster = lSkinDeformer->GetCluster(clusterIndex);
						mIndices = lCluster->GetControlPointIndices();

						if (!mIndices)
							continue;

						name = lCluster->GetNameOnly();

						asset->GetAnimationMatrix(submeshIDX, clusterIndex, translate);

						result = matrix.MultT(vertices[mIndices[0]]);

						Bones[name] = translate;

					} // ClusterCount
				} // SubmeshCount
			}
		}

	}

	void DynamicObjectData::GetAllSockets(std::unordered_map<std::string, float4x4>& Socket)
	{

	}

#pragma endregion

#pragma endregion
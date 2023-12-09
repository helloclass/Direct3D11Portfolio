#pragma once

#pragma warning(disable:4996)

#include <iostream>

#ifndef PHYSX_SAMPLE_H

#define PHYSX_SAMPLE_H

#include "PxPhysicsAPI.h"
#include <extensions/PxExtensionsAPI.h>
#include <extensions/PxDefaultErrorCallback.h>

#include "3DMaths.h"

#endif

using namespace physx;

class Physics {
protected:
	PxDefaultErrorCallback gErrorCallback;
	PxDefaultAllocator gAllocator;

	PxFoundation* gFoundation;
	PxPvd* gPvd;
	PxPhysics* gPhysics;
	PxCooking* gCooking;

	PxDefaultCpuDispatcher* gDispatcher;

	PxScene* gScene;

	PxMaterial* gMaterial;

public:
	PxPhysics* GetPhysics() const
	{
		return gPhysics;
	}

	PxScene* GetScene() const
	{
		return gScene;
	}

public:
	void Init();
	void Update();
	void CleanUp();

	PxRigidStatic* CreateStatic(const PxTransform& t, PxShape* shape);
	PxRigidDynamic* CreateKinematic(const PxTransform& t, PxShape* shape, float density, const PxVec3& velocity);
	PxRigidDynamic* CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity);

	PxShape* CreateBox(float x, float y, float z);
	PxShape* CreateBox(float3& Scale);
	PxShape* CreateSphere(float r);
	PxConvexMesh* CreateConvexMesh(const PxConvexMeshDesc& convexDesc);
	PxTriangleMesh* CreateTriangleMesh(const PxTriangleMeshDesc& meshDesc);
	PxShape* CreateCapsule(float r);
	void CreateStack(PxShape* shape, const PxTransform& t, PxU32 size, PxReal halfExtent);
	PxShape* CreateConvexMeshToShape(const PxConvexMeshDesc& convexDesc);
	PxShape* CreateTriangleMeshToShape(const PxTriangleMeshDesc& meshDesc);

	void setPosition(PxRigidDynamic* collider, float x, float y, float z);
	void setRotation(PxRigidDynamic* collider, float x, float y, float z);
	void setVelocity(PxRigidDynamic* collider, float x, float y, float z);
	void setTorque(PxRigidDynamic* collider, float x, float y, float z);

public:
	void SetGravity(float x, float y, float z)
	{
		PxVec3 gravity = PxVec3(x, y, z);

		gScene->setGravity(gravity);
	}

};

class PhysxPool {
private:
	static PhysxPool instance;

public:
	static PhysxPool& getInstance() {
		return instance;
	}

public:
	Physics data;
	// 플레이 상태인지 아닌지
	bool IsPlay = false;

public:
	bool Play()
	{
		IsPlay = true;

		data.SetGravity(0.0f, -9.8f, 0.0f);
	}

	bool Stop()
	{
		IsPlay = false;

		data.SetGravity(0.0f, 0.0f, 0.0f);
	}
};
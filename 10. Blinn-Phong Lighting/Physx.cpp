#include "Physx.h"

PhysxPool PhysxPool::instance;

void Physics::Init()
{
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, 10.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(4);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);

	//PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	//gScene->addActor(*groundPlane);

}

void Physics::Update() {
	gScene->simulate(1.0f / 30.0f);
	gScene->fetchResults(true);
}

void Physics::CleanUp() {
	try {
		gScene->release();
		gFoundation->release();
		gMaterial->release();
		gDispatcher->release();
		gPhysics->release();
		gCooking->release();

		if (gPvd)
		{
			PxPvdTransport* transport = gPvd->getTransport();
			gPvd->release();
			transport->release();
		}

		//for (int bindObjCnt = 0; bindObjCnt < bindObj.size(); bindObjCnt++)
		//	delete(bindObj[bindObjCnt]);

		//for (int colliderCnt = 0; colliderCnt < StaticColliders.size(); colliderCnt++)
		//	StaticColliders[colliderCnt]->release();

		//for (int colliderCnt = 0; colliderCnt < DynamicColliders.size(); colliderCnt++)
		//	DynamicColliders[colliderCnt]->release();
	}
	catch (std::exception)
	{
		throw std::runtime_error("");
	}
}

PxShape* Physics::CreateBox(float Scale_x, float Scale_y, float Scale_z) {
	PxShape* shape = gPhysics->createShape(
		PxBoxGeometry(Scale_x, Scale_y, Scale_z),
		*gMaterial
	);

	return shape;
}
PxShape* Physics::CreateBox(float3& Scale) {
	PxShape* shape = gPhysics->createShape(
		PxBoxGeometry(Scale.x, Scale.y, Scale.z),
		*gMaterial
	);

	return shape;
}

PxShape* Physics::CreateSphere(float r) {
	PxShape* shape = gPhysics->createShape(
		PxSphereGeometry(r), 
		*gMaterial
	);

	return shape;
}

PxConvexMesh* Physics::CreateConvexMesh(const PxConvexMeshDesc& convexDesc) {
	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!gCooking->cookConvexMesh(convexDesc, buf, &result))
		return NULL;
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* convexMesh = gPhysics->createConvexMesh(input);

	return convexMesh;
}

PxTriangleMesh* Physics::CreateTriangleMesh(const PxTriangleMeshDesc& meshDesc) {
	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = gCooking->cookTriangleMesh(meshDesc, writeBuffer, &result);

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	PxTriangleMesh* mesh = gPhysics->createTriangleMesh(readBuffer);
	return mesh;

	physx::PxMeshScale scale(physx::PxVec3(1.0f, 1.0f, -1.0f), physx::PxQuat(physx::PxIdentity));
	gPhysics->createShape(physx::PxTriangleMeshGeometry(mesh, scale), *gMaterial, true);
}

PxShape* Physics::CreateCapsule(float r) {
	PxRigidDynamic* aCapsuleActor = gPhysics->createRigidDynamic(PxTransform(0.0f, 0.0f, 0.0f));
	PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
	PxShape* shape = PxRigidActorExt::createExclusiveShape(
		*aCapsuleActor,
		PxCapsuleGeometry(r, r),
		*gMaterial);
	return shape;
}

void Physics::CreateStack(PxShape* shape, const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}

PxShape* Physics::CreateConvexMeshToShape(const PxConvexMeshDesc& convexDesc) {
	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!gCooking->cookConvexMesh(convexDesc, buf, &result))
		return NULL;
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* mesh = gPhysics->createConvexMesh(input);

	physx::PxMeshScale scale(physx::PxVec3(1.0f, 1.0f, -1.0f), physx::PxQuat(physx::PxIdentity));

	return gPhysics->createShape(physx::PxConvexMeshGeometry(mesh, scale), *gMaterial);
}

PxShape* Physics::CreateTriangleMeshToShape(const PxTriangleMeshDesc& meshDesc) {
	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = gCooking->cookTriangleMesh(meshDesc, writeBuffer, &result);

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	PxTriangleMesh* mesh = gPhysics->createTriangleMesh(readBuffer);

	physx::PxMeshScale scale(physx::PxVec3(1.0f, 1.0f, -1.0f), physx::PxQuat(physx::PxIdentity));
	physx::PxTriangleMeshGeometry meshGeom(mesh, scale);

	return gPhysics->createShape(meshGeom, *gMaterial);
}

PxRigidStatic* Physics::CreateStatic(const PxTransform& t, PxShape* shape)
{
	PxRigidStatic* staticMesh = PxCreateStatic(*gPhysics, t, *shape);
	if (!staticMesh)
		throw std::runtime_error("Create Static Failed..");

	gScene->addActor(*staticMesh);

	return staticMesh;
}

PxRigidDynamic* Physics::CreateKinematic(const PxTransform& t, PxShape* shape, float density , const PxVec3& velocity)
{
	PxRigidDynamic* kinematic = PxCreateKinematic(*gPhysics, t, *shape, density);
	if (!kinematic)
		throw std::runtime_error("Create Kinematic Failed..");

	kinematic->setLinearVelocity(velocity);
	gScene->addActor(*kinematic);

	return kinematic;
}

PxRigidDynamic* Physics::CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = 
		PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 0.1f);
	if (!dynamic)
		throw std::runtime_error("Create Dynamic Failed..");

	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);

	return dynamic;
}


void Physics::setPosition(PxRigidDynamic* collider, float x, float y, float z) {
	PxTransform pos = collider->getGlobalPose();
	pos.p.x = x;
	pos.p.y = y;
	pos.p.z = z;

	collider->setGlobalPose(pos);
}
void Physics::setRotation(PxRigidDynamic* collider, float x, float y, float z) {
	PxTransform pos = collider->getGlobalPose();

	float angle[3] = { x, y, z };
	PxQuat XQ(x, PxVec3(1, 0, 0));
	PxQuat YQ(y, PxVec3(0, 1, 0));
	PxQuat ZQ(z, PxVec3(0, 0, 1));

	PxQuat RQ = XQ * YQ * ZQ;

	pos.q.x = RQ.x;
	pos.q.y = RQ.y;
	pos.q.z = RQ.z;
	pos.q.w = RQ.w;

	collider->setGlobalPose(pos);
}
void Physics::setVelocity(PxRigidDynamic* collider, float x, float y, float z)
{
	collider->setLinearVelocity(PxVec3(x, y, z));
}
void Physics::setTorque(PxRigidDynamic* collider, float x, float y, float z)
{
	collider->setAngularVelocity(PxVec3(x, y, z));
}

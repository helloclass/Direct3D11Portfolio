#include "ParticleObjectData.h"

//int ParticlePhysics::_MAX_PARTICLE_COUNT = 0;


/*
	���� ���� �б�

	PxParticleBase::particleReadDataFlags�� ���� ������ �о���� ����

	PxParticleReadDataFlag::ePOSITION_BUFFER			(R)
	PxParticleReadDataFlag::eFLAGS_BUFFER				(R)
	PxParticleReadDataFlag::eVELOCITY_BUFFER			()
	PxParticleReadDataFlag::eREST_OFFSET_BUFFER			()
	PxParticleReadDataFlag::eCOLLISION_NORMAL_BUFFER	()
	PxParticleReadDataFlag::eDENSITY_BUFFER				()

	eFLAGS_BUFFER�� ���� ������ ���� �����͸� ���� �� �ִ�

	PxParticleFlag::eVALID (����� Index �������� �ƴ���)
	PxParticleFlag::eCOLLISION_WITH_STATIC (������ �ùķ��̼ǿ��� ���ڰ� ��ü�� �浹�ߴ��� ����)
	PxParticleFlag::eCOLLISION_WITH_DYNAMIC (������ �ùķ��̼ǿ��� ���ڰ� ���� ��ü�� �浹�ߴ��� ����)
	
*/

// Physx�� ���� ��ƼŬ�� �а� ���� �𵨿� ������Ʈ
// ��Ƽ �������� ���� �������� Reading�� Independence�� ���� Writing�� ���� ���� 

struct UpdateParticlePhysicsParmaDesc {
	ParticlePhysics* ptr;
	int Index;

	UpdateParticlePhysicsParmaDesc(ParticlePhysics* ptr, int Index)
	{
		this->ptr = ptr;
		this->Index = Index;
	}
};

DWORD WINAPI UpdateParticlePhysics(PVOID param)
{
	UpdateParticlePhysicsParmaDesc* data = (UpdateParticlePhysicsParmaDesc*)(param);

	PxParticleReadData* rd;
	unsigned i;

	ParticleWorker* worker = data->ptr->ThreadPool[data->Index];

	WaitForSingleObject(worker->ParticleWriteEvent, INFINITE);

	while (true)
	{
		// ���� �����Ϳ� ������ �ϴ� ���
		// ��ƼŬ ������ �а� ��� ���°� �� �����
		rd = worker->ParticleSystem->lockParticleReadData();

		// access particle data from PxParticleReadData
		if (rd)
		{
			PxStrideIterator<const PxParticleFlags> flagsIt(rd->flagsBuffer);
			PxStrideIterator<const PxVec3> positionIt(rd->positionBuffer);

			for (i = 0; i < rd->validParticleRange; ++i, ++flagsIt, ++positionIt)
			{
				// ���� ������ �ε��� �����̶��
				if (*(flagsIt + i) & PxParticleFlag::eVALID)
				{
					// ������Ʈ �� Position ���� ��´�.
					worker->ParticlePositionBuffer[i] = *(positionIt + i);
				}
			}

			// ���� �ڿ��� �ٽ� ������ �����ϵ��� unlock
			rd->unlock();
		}

		ResetEvent(worker->ParticleWriteEvent);
		SetEvent(worker->ParticleReadEvent);

		WaitForSingleObject(worker->ParticleWriteEvent, INFINITE);
	}

	return 0;
}

ParticlePhysics::ParticlePhysics(std::vector<unsigned>& ParticleNum)
{
	UINT workerIter = 0;
	UINT i = 0;

	for (i = 0; i < ParticleNum.size(); i++)
	{
		this->ParticleNumList.push_back(ParticleNum[i]);
	}

	// Resize
	ThreadPool.resize(ParticleNumList.size());

	// _MAX_PARTICLE_WORKER ��ŭ ��ƼŬ �ý��۰� Ǯ�� ���� ����, ��Ŀ ������� R/W �̺�Ʈ�� ����
	// ���⼭ ������Ʈ�� ������ �ִ� _MAX_PARTICLE_WORKER ��ŭ �� �䱸�� �� �� ����.
	for (workerIter = 0; workerIter < ParticleNumList.size(); workerIter++)
	{
		// ��ƼŬ �ý��� ����
		bool perParticleRestOffset = false;

		ThreadPool[workerIter] = new ParticleWorker(ParticleNumList[workerIter]);

		ThreadPool[workerIter]->ParticleSystem =
			PhysxPool::getInstance().data.GetPhysics()->createParticleSystem(
				ParticleNumList[workerIter], perParticleRestOffset);

		if (ThreadPool[workerIter]->ParticleSystem)
			PhysxPool::getInstance().data.GetScene()->addActor(*ThreadPool[workerIter]->ParticleSystem);

		for (i = 0; i < ParticleNumList[workerIter]; i++)
		{
			ThreadPool[workerIter]->ParticleIndexBuffer.push_back(PxU32(i));
			ThreadPool[workerIter]->ParticlePositionBuffer.push_back(PxVec3(0.0f, 0.0f, 0.0f));
			ThreadPool[workerIter]->ParticleVelocityBuffer.push_back(PxVec3(0.0f, 0.0f, 0.0f));
		}

		PxParticleCreationData particleCreationData;
		particleCreationData.numParticles = ParticleNumList[workerIter];
		particleCreationData.indexBuffer = PxStrideIterator<const PxU32>(ThreadPool[workerIter]->ParticleIndexBuffer.data());
		particleCreationData.positionBuffer = PxStrideIterator<const PxVec3>(ThreadPool[workerIter]->ParticlePositionBuffer.data());
		particleCreationData.velocityBuffer = PxStrideIterator<const PxVec3>(ThreadPool[workerIter]->ParticleVelocityBuffer.data());

		assert(ThreadPool[workerIter]->ParticleSystem->createParticles(particleCreationData));

		// �ε��� Ǯ ����
		ThreadPool[workerIter]->ParticleIndexPool = PxParticleExt::createIndexPool(ParticleNumList[workerIter]);
		ThreadPool[workerIter]->ParticleIndexPool->allocateIndices(
			ParticleNumList[workerIter],
			PxStrideIterator<PxU32>(ThreadPool[workerIter]->ParticleIndexBuffer.data())
		);

		// Worker Event ����
		ThreadPool[workerIter]->ParticleReadEvent =
			CreateEvent(NULL, FALSE, FALSE, NULL);
		ThreadPool[workerIter]->ParticleWriteEvent =
			CreateEvent(NULL, FALSE, FALSE, NULL);

		SetEvent(ThreadPool[workerIter]->ParticleReadEvent);
		ResetEvent(ThreadPool[workerIter]->ParticleWriteEvent);

		// Worker Thread ����
		ThreadPool[workerIter]->UpdateThreadFunc =
			CreateThread(NULL, 0, UpdateParticlePhysics, new UpdateParticlePhysicsParmaDesc(this, workerIter), NULL, &ThreadPool[workerIter]->UpdateThreadID);

		assert(ThreadPool[workerIter]->UpdateThreadFunc && "Create Thread Failed...");
	}
}

ParticlePhysics::~ParticlePhysics()
{
	for (UINT workerIter = 0; workerIter < ParticleNumList.size(); workerIter++)
	{
		if (ThreadPool[workerIter]->UpdateThreadFunc)
		{
			// ������ ��ƼŬ�� ������ ��Ű�� �ٸ� ��ƼŬ�� �ٽ� reuse �մϴ�.
			ThreadPool[workerIter]->ParticleIndexPool->freeIndices(
				ParticleNumList[workerIter],
				PxStrideIterator<PxU32>(ThreadPool[workerIter]->ParticleIndexBuffer.data())
			);
			// Ǯ�� ������ �մϴ�.
			ThreadPool[workerIter]->ParticleIndexPool->release();

			// Totally Release
			ThreadPool[workerIter]->ParticleSystem->releaseParticles();

			// ������ ����
			GetExitCodeThread(
				ThreadPool[workerIter]->UpdateThreadFunc,
				&ThreadPool[workerIter]->UpdateThreadID
			);
			CloseHandle(ThreadPool[workerIter]->UpdateThreadFunc);

			CloseHandle(ThreadPool[workerIter]->ParticleReadEvent);
			CloseHandle(ThreadPool[workerIter]->ParticleWriteEvent);
		}

		//delete (ThreadPool[workerIter]->ParticleIndexBuffer);
		//delete (ThreadPool[workerIter]->ParticlePositionBuffer);
		//delete (ThreadPool[workerIter]->ParticleVelocityBuffer);
	}
}

void ParticlePhysics::ParticleUpdate()
{
	std::vector<HANDLE> ThreadWorkers;
	for (UINT workerIter = 0; workerIter < ParticleNumList.size(); workerIter++)
	{
		//// �� ��ƼŬ�� �����Ͽ� �ӵ��� ���� ��Ű�� �Լ�
		//ThreadPool[workerIter]->ParticleSystem->addForces(
		//	ParticleNumList[workerIter],
		//	PxStrideIterator<PxU32>(ThreadPool[workerIter]->ParticleIndexBuffer.data()),
		//	PxStrideIterator<PxVec3>(ThreadPool[workerIter]->ParticleVelocityBuffer.data()),
		//	PxForceMode::eFORCE
		//);

		ResetEvent(ThreadPool[workerIter]->ParticleReadEvent);
		SetEvent(ThreadPool[workerIter]->ParticleWriteEvent);

		ThreadWorkers.push_back(ThreadPool[workerIter]->ParticleReadEvent);
	}

	WaitForMultipleObjects(ThreadWorkers.size(), ThreadWorkers.data(), true, INFINITE);

}






struct ClothVertex
{
	// Particle Index
	UINT Hash;
	float Weight;

	float3 Position;
};

struct ClothEdge
{
	ClothVertex* Vert1;
	ClothVertex* Vert2;

	float Gamma;
	float3 Normal;

	float Length0;
};

struct ClothFace
{
	std::vector<ClothEdge*> Edges;
};

std::vector<ClothFace*> ClothFaces;

void GetRamda(ClothFace* face, float alpha)
{
	int i, j;
	float len;

	float C;
	float Divider;

	for (i = 0; i < face->Edges.size(); i++)
	{
		len = length(face->Edges[i]->Vert2->Position - face->Edges[i]->Vert1->Position);

		C = len - face->Edges[i]->Length0;
		Divider = 0.0f;

		for (j = 0; j < face->Edges.size(); j++)
		{
			Divider += (
				(1.0f / face->Edges[i]->Vert1->Weight) *
				pow(length(face->Edges[i]->Normal), 2.0f)
			);

			Divider += (
				(1.0f / face->Edges[i]->Vert2->Weight) *
				pow(length(face->Edges[i]->Normal), 2.0f)
			);
		}

		Divider += (alpha / pow(GlobalTime::getInstance().dt, 2.0f));

		face->Edges[i]->Gamma = -C / Divider;
	}
}

void Solve()
{
	int i;
	float3 delta;

	for (ClothFace* face : ClothFaces)
	{
		for (i = 0; i < face->Edges.size(); i++)
		{
			// Update Normal Vector
			face->Edges[i]->Normal =
				normalise(cross(face->Edges[i]->Vert2->Position, face->Edges[i]->Vert1->Position));

			// Solve
			GetRamda(face, 1.0f);

			face->Edges[i]->Vert1->Position +=
				face->Edges[i]->Gamma *
				1.0f / face->Edges[i]->Vert1->Weight *
				face->Edges[i]->Normal;

			face->Edges[i]->Vert2->Position +=
				face->Edges[i]->Gamma *
				1.0f / face->Edges[i]->Vert2->Weight *
				face->Edges[i]->Normal;
		}
	}
}
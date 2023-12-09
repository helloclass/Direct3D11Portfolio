#include "ParticleObjectData.h"

//int ParticlePhysics::_MAX_PARTICLE_COUNT = 0;


/*
	입자 상태 읽기

	PxParticleBase::particleReadDataFlags를 통해 정보를 읽어오기 가능

	PxParticleReadDataFlag::ePOSITION_BUFFER			(R)
	PxParticleReadDataFlag::eFLAGS_BUFFER				(R)
	PxParticleReadDataFlag::eVELOCITY_BUFFER			()
	PxParticleReadDataFlag::eREST_OFFSET_BUFFER			()
	PxParticleReadDataFlag::eCOLLISION_NORMAL_BUFFER	()
	PxParticleReadDataFlag::eDENSITY_BUFFER				()

	eFLAGS_BUFFER로 부터 다음과 같은 데이터를 읽을 수 있다

	PxParticleFlag::eVALID (예약된 Index 공간인지 아닌지)
	PxParticleFlag::eCOLLISION_WITH_STATIC (마지막 시뮬레이션에서 입자가 강체와 충돌했는지 여부)
	PxParticleFlag::eCOLLISION_WITH_DYNAMIC (마지막 시뮬레이션에서 입자가 동적 강체와 충돌했는지 여부)
	
*/

// Physx로 부터 파티클을 읽고 실제 모델에 업데이트
// 멀티 스레딩을 통한 안정적인 Reading과 Independence를 위해 Writing은 절대 금지 

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
		// 입자 데이터에 엑세스 하는 방식
		// 파티클 데이터 읽게 잠깐 쓰는것 좀 멈춰봐
		rd = worker->ParticleSystem->lockParticleReadData();

		// access particle data from PxParticleReadData
		if (rd)
		{
			PxStrideIterator<const PxParticleFlags> flagsIt(rd->flagsBuffer);
			PxStrideIterator<const PxVec3> positionIt(rd->positionBuffer);

			for (i = 0; i < rd->validParticleRange; ++i, ++flagsIt, ++positionIt)
			{
				// 접근 가능한 인덱스 공간이라면
				if (*(flagsIt + i) & PxParticleFlag::eVALID)
				{
					// 업데이트 된 Position 값을 얻는다.
					worker->ParticlePositionBuffer[i] = *(positionIt + i);
				}
			}

			// 읽은 뒤에는 다시 편집이 가능하도록 unlock
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

	// _MAX_PARTICLE_WORKER 만큼 파티클 시스템과 풀을 만든 다음, 워커 스레드와 R/W 이벤트를 제작
	// 여기서 오브젝트의 개수가 최대 _MAX_PARTICLE_WORKER 만큼 더 요구가 될 수 있음.
	for (workerIter = 0; workerIter < ParticleNumList.size(); workerIter++)
	{
		// 파티클 시스템 제작
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

		// 인덱스 풀 생성
		ThreadPool[workerIter]->ParticleIndexPool = PxParticleExt::createIndexPool(ParticleNumList[workerIter]);
		ThreadPool[workerIter]->ParticleIndexPool->allocateIndices(
			ParticleNumList[workerIter],
			PxStrideIterator<PxU32>(ThreadPool[workerIter]->ParticleIndexBuffer.data())
		);

		// Worker Event 생성
		ThreadPool[workerIter]->ParticleReadEvent =
			CreateEvent(NULL, FALSE, FALSE, NULL);
		ThreadPool[workerIter]->ParticleWriteEvent =
			CreateEvent(NULL, FALSE, FALSE, NULL);

		SetEvent(ThreadPool[workerIter]->ParticleReadEvent);
		ResetEvent(ThreadPool[workerIter]->ParticleWriteEvent);

		// Worker Thread 생성
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
			// 이전의 파티클을 릴리즈 시키고 다른 파티클을 다시 reuse 합니다.
			ThreadPool[workerIter]->ParticleIndexPool->freeIndices(
				ParticleNumList[workerIter],
				PxStrideIterator<PxU32>(ThreadPool[workerIter]->ParticleIndexBuffer.data())
			);
			// 풀을 릴리즈 합니다.
			ThreadPool[workerIter]->ParticleIndexPool->release();

			// Totally Release
			ThreadPool[workerIter]->ParticleSystem->releaseParticles();

			// 쓰레드 종료
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
		//// 각 파티클에 접근하여 속도를 가속 시키는 함수
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
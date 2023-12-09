#pragma once
#include "Memory.h"

class Level
{
public:
	bool SaveLevel(std::string name)
	{
		std::string DirPath = "MetaData\\Level";
		std::string Path;

		if (!name.empty())
		{
			Path = DirPath + "\\" + name;
		}
		else
		{
			return false;
		}

		// 폴더 생성
		CreateFolders(DirPath);

		// 파일 오픈 및 생성
		std::ofstream LevelFile;
		LevelFile.open(Path, std::ios::out | std::ios::binary);
		if (!LevelFile.is_open())
		{
			MessageBoxA(0, "Create File Failed", "Warning", MB_OK);
			return false;
		}

		// 저장할 게임 오브젝트들 불러오기
		MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_;
		MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_;

		for (std::pair<std::string, ObjectData*> obj : MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_)
		{
			LevelFile << obj.first;
			LevelFile << obj.second->transform->InstanceCount;

			for (std::pair<std::string, Transform::transform*> inst : obj.second->transform->InstanceBuffer)
			{
				// 글로벌 (월드) 좌표계
				LevelFile << inst.second->GlobalMatrix->Position[0];
				LevelFile << inst.second->GlobalMatrix->Position[1];
				LevelFile << inst.second->GlobalMatrix->Position[2];

				LevelFile << inst.second->GlobalMatrix->Rotation[0];
				LevelFile << inst.second->GlobalMatrix->Rotation[1];
				LevelFile << inst.second->GlobalMatrix->Rotation[2];

				LevelFile << inst.second->GlobalMatrix->Scale[0];
				LevelFile << inst.second->GlobalMatrix->Scale[1];
				LevelFile << inst.second->GlobalMatrix->Scale[2];

				// 로컬 (국소) 좌표계
				LevelFile << inst.second->LocalMatrix->Position[0];
				LevelFile << inst.second->LocalMatrix->Position[1];
				LevelFile << inst.second->LocalMatrix->Position[2];

				LevelFile << inst.second->LocalMatrix->Rotation[0];
				LevelFile << inst.second->LocalMatrix->Rotation[1];
				LevelFile << inst.second->LocalMatrix->Rotation[2];

				LevelFile << inst.second->LocalMatrix->Scale[0];
				LevelFile << inst.second->LocalMatrix->Scale[1];
				LevelFile << inst.second->LocalMatrix->Scale[2];

				// 속도
				LevelFile << inst.second->Velocity->x;
				LevelFile << inst.second->Velocity->y;
				LevelFile << inst.second->Velocity->z;

				// 토크
				LevelFile << inst.second->Torque->x;
				LevelFile << inst.second->Torque->y;
				LevelFile << inst.second->Torque->z;

				// 가속도
				LevelFile << inst.second->Accumulate->x;
				LevelFile << inst.second->Accumulate->y;
				LevelFile << inst.second->Accumulate->z;

			}
		}

		LevelFile.close();
	}

	bool LoadLevel(std::string name)
	{

	}
};


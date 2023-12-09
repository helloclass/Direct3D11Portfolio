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

		// ���� ����
		CreateFolders(DirPath);

		// ���� ���� �� ����
		std::ofstream LevelFile;
		LevelFile.open(Path, std::ios::out | std::ios::binary);
		if (!LevelFile.is_open())
		{
			MessageBoxA(0, "Create File Failed", "Warning", MB_OK);
			return false;
		}

		// ������ ���� ������Ʈ�� �ҷ�����
		MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_;
		MemoryPool::getInstance()._GLOBAL_LIGHT_POOL_;

		for (std::pair<std::string, ObjectData*> obj : MemoryPool::getInstance()._GLOBAL_OBJECT_POOL_)
		{
			LevelFile << obj.first;
			LevelFile << obj.second->transform->InstanceCount;

			for (std::pair<std::string, Transform::transform*> inst : obj.second->transform->InstanceBuffer)
			{
				// �۷ι� (����) ��ǥ��
				LevelFile << inst.second->GlobalMatrix->Position[0];
				LevelFile << inst.second->GlobalMatrix->Position[1];
				LevelFile << inst.second->GlobalMatrix->Position[2];

				LevelFile << inst.second->GlobalMatrix->Rotation[0];
				LevelFile << inst.second->GlobalMatrix->Rotation[1];
				LevelFile << inst.second->GlobalMatrix->Rotation[2];

				LevelFile << inst.second->GlobalMatrix->Scale[0];
				LevelFile << inst.second->GlobalMatrix->Scale[1];
				LevelFile << inst.second->GlobalMatrix->Scale[2];

				// ���� (����) ��ǥ��
				LevelFile << inst.second->LocalMatrix->Position[0];
				LevelFile << inst.second->LocalMatrix->Position[1];
				LevelFile << inst.second->LocalMatrix->Position[2];

				LevelFile << inst.second->LocalMatrix->Rotation[0];
				LevelFile << inst.second->LocalMatrix->Rotation[1];
				LevelFile << inst.second->LocalMatrix->Rotation[2];

				LevelFile << inst.second->LocalMatrix->Scale[0];
				LevelFile << inst.second->LocalMatrix->Scale[1];
				LevelFile << inst.second->LocalMatrix->Scale[2];

				// �ӵ�
				LevelFile << inst.second->Velocity->x;
				LevelFile << inst.second->Velocity->y;
				LevelFile << inst.second->Velocity->z;

				// ��ũ
				LevelFile << inst.second->Torque->x;
				LevelFile << inst.second->Torque->y;
				LevelFile << inst.second->Torque->z;

				// ���ӵ�
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


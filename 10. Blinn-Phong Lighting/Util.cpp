#include "Util.h"
#include <filesystem>

GlobalTime GlobalTime::gt;
GlobalKeyVector GlobalKeyVector::kv;

#pragma region UI_HWND

void UI_HWND::setVisible(bool isVisible)
{
	if (!isVisible)
		ShowWindow(hwnd, SW_HIDE);
	else
		ShowWindow(hwnd, SW_SHOW);
}

#pragma endregion

std::string Path2FileName(const char* texIter)
{
	std::string TexName;

	UINT texOffset = 0;
	UINT texSize = strlen(texIter);

	for (int t = texSize - 1; t >= 0; t--)
	{
		if ((texIter)[t] == '/' or (texIter)[t] == '\\')
		{
			texOffset = t + 1;
			break;
		}
	}

	for (int t = texOffset; t < texSize; t++)
		TexName += (texIter)[t];

	return TexName;
}

void CreateFolders(std::string path)
{
	std::filesystem::path p(path);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);
}

// ��Ʈ ���� ��� ���� �н��� ����
void GetAllSubFolders(std::string root, std::vector<std::string>& Folders)
{
	for (const auto& file : directory_iterator(root))
	{
		Folders.push_back(file.path().string());
	}
}

/**
*
* �ִϸ��̼ǿ� ��Ÿ ������ ������ ���� �������� ����ȴ�.
* �� B0, B1, B2 �� ���� �� B0�� ��Ÿ�� ����ȴ�. �̶� �WŸ�� ����� B2��?
*
* B2_ = Vn * B2 * (B0)^-1 * B0_delta * B0
*
* �̶� ����� ���� "���չ�Ģ"�� �����ȴ�.
*
* B2_ = Vn * B2 * ((B0)^-1 * B0_delta * B0)
*
* Q> ��Ÿ�� ���ؾ� �ϴ� ������?
*
*/

// ���� ��Ÿ�� ������Ʈ �Ѵ�.
// 
// ����� �״��� ���� ����.
// ��ĳ����� ���� ���չ�Ģ�� �����ǹǷ� ��� ���ؽ��� ���ؼ� ���ο� ������ �� �ʿ䰡 ����.
// ���� ȸ�� => ���� ����
//
void ObjectData::UpdateBoneDelta()
{
	Tree<Skeleton>* Node = nullptr;
	std::queue<Tree<Skeleton>*> skeletons;
	skeletons.push(&BoneHierarchy);

	int qSize = 0;

	while (skeletons.size())
	{
		qSize = skeletons.size();

		// ���� ������ ���̷��� ���� ��ŭ
		while (qSize--)
		{
			Node = skeletons.front();
			skeletons.pop();

			Node->Item.DeltaWeightMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};

			// ��Ÿ ����ġ ��Ʈ���� ����
			if (Node->Parent)
			{
				Node->Item.DeltaWeightMatrix =
					// ((Bone_Mat)^-1 * Bone_Delta * Bone_Mat)
					inverse(Node->Item.TranslationMatrix) *
					Node->Item.DeltaTranslationMatrix *
					Node->Item.TranslationMatrix *

					// Previous_Bone_Weight_Matrix
					// �ڼ� ����� �θ� ���뿡 ���ӵǾ� ����
					Node->Parent->Item.DeltaWeightMatrix; 
			}

			// ���� ���ϵ� �ҷ�����
			for (Tree<Skeleton>* child : Node->Childs)
				skeletons.push(child);
		}
	}
}

// ���� ������ �Ȱ����� true ���� �ٸ��� false
bool ObjectData::CheckSibilingSkeletal(ObjectData* target)
{
	Tree<Skeleton>* Node = nullptr;
	Tree<Skeleton>* TargetNode = nullptr;

	std::queue<Tree<Skeleton>*> skeletons;
	std::queue<Tree<Skeleton>*> TargetSkeletons;

	skeletons.push(&BoneHierarchy);
	TargetSkeletons.push(&target->BoneHierarchy);

	int qSize = 0;

	while (skeletons.size())
	{
		qSize = skeletons.size();

		// ���� ������ ���̷��� ���� ��ŭ
		while (qSize--)
		{
			Node = skeletons.front();
			skeletons.pop();

			TargetNode = TargetSkeletons.front();
			TargetSkeletons.pop();

			// ���� ������ �ٸ��ٸ�
			if (Node->Childs.size() != TargetNode->Childs.size())
				return false;

			// ���� ���ϵ� �ҷ�����
			for (Tree<Skeleton>* child : Node->Childs)
				skeletons.push(child);
			for (Tree<Skeleton>* child : TargetNode->Childs)
				TargetSkeletons.push(child);
		}
	}

	return true;
}

bool ObjectData::CheckSibilingMesh(ObjectData* target)
{
	// �켱 ����޽� ���� ���� ������ Ȯ��
	if (SubmeshCount != target->SubmeshCount)
		return false;

	// ����޽��� ���ٸ� 
	// ����޽� ��ŭ ������ ������, ���ؽ��� �ε����� �����Ѵ�.
	int VertexCompare1, VertexCompare2;
	int IndexCompare1, IndexCompare2;

	for (int submeshIDX = 0; submeshIDX < SubmeshCount; submeshIDX++)
	{
		VertexCompare1 = meshDatas[submeshIDX].Vertices.size();
		VertexCompare2 = target->meshDatas[submeshIDX].Vertices.size();

		if (VertexCompare1 != VertexCompare2)
			return false;

		IndexCompare1 = meshDatas[submeshIDX].Indices32.size();
		IndexCompare2 = target->meshDatas[submeshIDX].Indices32.size();

		if (IndexCompare1 != IndexCompare2)
			return false;
	}

	return true;
}

bool ObjectData::ImportAnimationSequence(std::string Path)
{
	std::ifstream SequenceFile(Path, std::ifstream::binary);
	if (!SequenceFile) {
		MessageBoxA(0, "Sequence File Not Found", "Warning", MB_OK);
		return false;
	}

	UINT SequenceFrameString;
	SequenceFile >> SequenceFrameString;

	Sequence* newAnimation = CreateSequence();
	newAnimation->mName = Path2FileName(Path.c_str());

	newAnimation->mSubmeshSize = SubmeshCount;
	newAnimation->mFrameSize = SequenceFrameString;
	newAnimation->mClusterSize = BoneCount;

	newAnimation->mAnimVertexArrays = new float*** [SubmeshCount];
	for (int submeshIDX = 0; submeshIDX < SubmeshCount; submeshIDX++)
	{
		newAnimation->mAnimVertexArrays[submeshIDX] = new float** [SequenceFrameString];
		for (int FrameIDX = 0; FrameIDX < SequenceFrameString; FrameIDX++)
		{
			newAnimation->mAnimVertexArrays[submeshIDX][FrameIDX] = new float* [BoneCount];
			for (int ClusterIDX = 0; ClusterIDX < BoneCount; ClusterIDX++)
			{
				newAnimation->mAnimVertexArrays[submeshIDX][FrameIDX][ClusterIDX] = new float[16];
				for (int i = 0; i < 16; i++)
				{
					SequenceFile.read(
						reinterpret_cast<char*>(&newAnimation->mAnimVertexArrays[submeshIDX][FrameIDX][ClusterIDX][i]),
						sizeof(float)
					);
				}
			}
		}
	}

	newAnimation->mFrameLength = SequenceFrameString;



	SequenceFile.close();
	
	return true;
}

bool ObjectData::ImportAnimationMontage(std::string Path)
{
	std::ifstream MontageFile(Path, std::ifstream::binary);
	if (!MontageFile) {
		MessageBoxA(0, "Montage File Not Found", "Warning", MB_OK);
		return false;
	}

	std::string MontageName = Path2FileName(Path.c_str());

	Montages[MontageName].mName = MontageName;

	// ��Ÿ�ֿ� ����� ������ ���� �˾ƿ���
	UINT SequenceCount = 0;
	MontageFile >> SequenceCount;

	// ������ ������ŭ Ŭ�� ������ Ű���
	Montages[MontageName].mSequenceList.resize(SequenceCount);

	for (Montage::SequenceClip& clip : Montages[MontageName].mSequenceList)
	{
		// ������ �̸�
		MontageFile >> clip.mSequence->mName;
		// ������ ���� �ð�
		MontageFile >> clip.mClipStartFrame;
		// ������ �� �ð�
		MontageFile >> clip.mClipEndFrame;
	}

	MontageFile.close();

	return true;
}

bool ObjectData::ImportAnimationBlendSpace(std::string Path)
{
	std::ifstream BlendSpaceFile(Path, std::ifstream::binary);
	if (!BlendSpaceFile) {
		MessageBoxA(0, "BlendSpace File Not Found", "Warning", MB_OK);
		return false;
	}

	std::string BlendSpaceName = Path2FileName(Path.c_str());

	BlendSpaces[BlendSpaceName].mName = BlendSpaceName;

	// ���� �����̽��� ����� ������ ���� �� Weight �˾ƿ���
	UINT SequenceCount = 0;
	float WeightY, WeightX;

	BlendSpaceFile >> SequenceCount;
	BlendSpaceFile >> WeightX >> WeightY;

	// ������ ������ŭ Ŭ�� ������ Ű���
	BlendSpaces[BlendSpaceName].mClips.resize(SequenceCount);

	for (BlendSpace::SequenceClip& clip : BlendSpaces[BlendSpaceName].mClips)
	{
		// ������ �̸�
		BlendSpaceFile >> clip.mSequence->mName;
		// ������ ���� �ð�
		BlendSpaceFile >> clip.mPositionY;
		// ������ �� �ð�
		BlendSpaceFile >> clip.mPositionX;
	}

	BlendSpaceFile.close();

	return true;
}

bool ObjectData::ImportAnimationComposite(std::string Path)
{
	std::ifstream CompositeFile(Path, std::ifstream::binary);
	if (!CompositeFile) {
		MessageBoxA(0, "Composite File Not Found", "Warning", MB_OK);
		return false;
	}

	std::string CompositeName = Path2FileName(Path.c_str());

	Composites[CompositeName].mName = CompositeName;

	// ���� �����̽��� ����� ������ ���� �� Weight �˾ƿ���
	UINT SequenceCount = 0;
	float WeightY, WeightX;

	CompositeFile >> SequenceCount;
	CompositeFile >> WeightX >> WeightY;

	// ������ ������ŭ Ŭ�� ������ Ű���
	Composites[CompositeName].mClips.resize(SequenceCount);

	for (Composite::SequenceClip& clip : Composites[CompositeName].mClips)
	{
		// ������ �̸�
		CompositeFile >> clip.mSequence->mName;
		// ������ ���� �ð�
		CompositeFile >> clip.mPositionX;
		// ������ �� �ð�
		CompositeFile >> clip.mPositionY;
	}

	CompositeFile.close();

	return true;
}

bool ObjectData::ImportAnimationMorph(std::string Path)
{
	std::ifstream MorphFile(Path, std::ifstream::binary);
	if (!MorphFile) {
		MessageBoxA(0, "Morph File Not Found", "Warning", MB_OK);
		return false;
	}

	UINT SequenceFrameString;
	MorphFile >> SequenceFrameString;

	std::string MorphName = Path2FileName(Path.c_str());

	Morphes[MorphName].mName = MorphName;

	UINT SubmeshCount = 0;
	UINT VerticesCount = 0;
	
	int SubmeshIDX;
	int VertexIDX;

	// ����޽� ������ ����
	MorphFile >> SubmeshCount;
	Morphes[MorphName].mSecondVertex.resize(SubmeshCount);

	for (SubmeshIDX = 0; SubmeshIDX < SubmeshCount; SubmeshIDX++)
	{
		// ���ؽ� ������ ����
		MorphFile >> VerticesCount;
		Morphes[MorphName].mSecondVertex[SubmeshIDX].resize(VerticesCount);

		for (VertexIDX = 0; VertexIDX < VerticesCount; VertexIDX++)
		{
			MorphFile >> Morphes[MorphName].mSecondVertex[SubmeshIDX][VertexIDX].x;
			MorphFile >> Morphes[MorphName].mSecondVertex[SubmeshIDX][VertexIDX].y;
			MorphFile >> Morphes[MorphName].mSecondVertex[SubmeshIDX][VertexIDX].z;
		}
	}

	MorphFile.close();

	return true;
}

bool ObjectData::ExportAnimationSequence(ObjectData* target, std::string name)
{
	if (!CheckSibilingSkeletal(target))
		return false;

	std::string DirPath = 
		"MetaData\\" + this->objectName + "\\Sequence";
	std::string Path;

	if (name.empty())
	{
		Path = DirPath + "\\" + target->objectName + ".seq";
	}
	else
	{
		Path = DirPath + "\\" + name + ".seq";
	}

	// ���� ����
	CreateFolders(DirPath);

	// ���� ���� �� ����
	std::ofstream SequenceFile;
	SequenceFile.open(Path, std::ios::out | std::ios::binary);
	if (!SequenceFile.is_open())
	{
		MessageBoxA(0, "Create File Failed", "Warning", MB_OK);
		return false;
	}

	int SequenceLength = target->Sequences[0]->mFrameLength;

	SequenceFile << SequenceLength;

	int submeshIDX, FrameIDX, ClusterIDX;
	int i;

	for (submeshIDX = 0; submeshIDX < target->SubmeshCount; submeshIDX++)
	{
		for (FrameIDX = 0; FrameIDX < SequenceLength; FrameIDX++)
		{
			for (ClusterIDX = 0; ClusterIDX < target->BoneCount; ClusterIDX++)
			{
				for (i = 0; i < 16; i++)
				{
					SequenceFile.write (
						reinterpret_cast<const char*>(&target->Sequences[0]->mAnimVertexArrays[submeshIDX][FrameIDX][ClusterIDX][i]),
						sizeof(float)
					);
				}
			}
		}
	}

	SequenceFile.close();

	return true;
}

bool ObjectData::ExportAnimationMontage()
{
	std::string DirPath =
		"MetaData\\" + this->objectName + "\\Montage";
	std::string Path;

	// ���� ����
	std::filesystem::path p(DirPath);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);

	// ���� ���� �� ����
	std::ofstream MontageFile;

	Montage* montage = nullptr;
	

	for (std::pair<std::string, Montage> mont : Montages)
	{
		montage = &mont.second;

		Path = DirPath + "\\" + montage->mName + ".mont";

		// Load or Create File
		MontageFile.open(Path, std::ios::out | std::ios::binary);
		if (!MontageFile.is_open())
		{
			MessageBoxA(0, "Create Montage File Failed", "Warning", MB_OK);
			return false;
		}

		// Start to write
		int SequenceCount = static_cast<int>(montage->mSequenceList.size());

		// ��Ÿ���� ������ ������ ����
		MontageFile << SequenceCount;

		for (Montage::SequenceClip clip : montage->mSequenceList)
		{
			// ������ �̸�
			MontageFile << clip.mSequence->mName;
			// ������ ���� �ð�
			MontageFile << clip.mClipStartFrame;
			// ������ �� �ð�
			MontageFile << clip.mClipEndFrame;
		}

		MontageFile.close();
	}

	return true;
}

bool ObjectData::ExportAnimationBlendSpace()
{
	std::string DirPath =
		"MetaData\\" + this->objectName + "\\BlendSpace";
	std::string Path;

	// ���� ����
	std::filesystem::path p(DirPath);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);

	// ���� ���� �� ����
	std::ofstream BlendSpaceFile;

	BlendSpace* blendspace = nullptr;


	for (std::pair<std::string, BlendSpace> bs : BlendSpaces)
	{
		blendspace = &bs.second;

		Path = DirPath + "\\" + blendspace->mName + ".bls";

		// Load or Create File
		BlendSpaceFile.open(Path, std::ios::out | std::ios::binary);
		if (!BlendSpaceFile.is_open())
		{
			MessageBoxA(0, "Create BlendSpace File Failed", "Warning", MB_OK);
			return false;
		}

		// Start to write
		int SequenceCount = static_cast<int>(blendspace->mClips.size());

		// ���� �����̽��� �������� ����
		BlendSpaceFile << SequenceCount;

		// ���� �����̽��� ����ġ�� ���´�.
		BlendSpaceFile << blendspace->mWeightY;
		BlendSpaceFile << blendspace->mWeightX;

		for (BlendSpace::SequenceClip clip : blendspace->mClips)
		{
			// ������ �̸�
			BlendSpaceFile << clip.mSequence->mName;
			// ������ ���� �ð�
			BlendSpaceFile << clip.mPositionY;
			// ������ �� �ð�
			BlendSpaceFile << clip.mPositionX;
		}

		BlendSpaceFile.close();
	}

	return true;
}

bool ObjectData::ExportAnimationComposite()
{
	std::string DirPath =
		"MetaData\\" + this->objectName + "\\Composite";
	std::string Path;

	// ���� ����
	std::filesystem::path p(DirPath);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);

	// ���� ���� �� ����
	std::ofstream CompositeFile;

	Composite* composite = nullptr;


	for (std::pair<std::string, Composite> c : Composites)
	{
		composite = &c.second;

		Path = DirPath + "\\" + composite->mName + ".com";

		// Load or Create File
		CompositeFile.open(Path, std::ios::out | std::ios::binary);
		if (!CompositeFile.is_open())
		{
			MessageBoxA(0, "Create Composite File Failed", "Warning", MB_OK);
			return false;
		}

		// Start to write
		int SequenceCount = static_cast<int>(composite->mClips.size());

		// ������Ʈ�� �������� ����
		CompositeFile << SequenceCount;

		// ������Ʈ�� ����ġ�� ���´�.
		CompositeFile << composite->mWeightY;
		CompositeFile << composite->mWeightX;

		for (Composite::SequenceClip clip : composite->mClips)
		{
			// ������ �̸�
			CompositeFile << clip.mSequence->mName;
			// ������ ���� �ð�
			CompositeFile << clip.mPositionY;
			// ������ �� �ð�
			CompositeFile << clip.mPositionX;
		}

		CompositeFile.close();
	}

	return true;
}

bool ObjectData::ExportAnimationMorph(ObjectData* target, std::string name)
{
	// �޽� ����
	if (!CheckSibilingMesh(target))
		return false;

	std::string DirPath =
		"MetaData\\" + this->objectName + "\\Morph";
	std::string Path;

	if (name.empty())
	{
		Path = DirPath + "\\" + target->objectName + ".mph";
	}
	else
	{
		Path = DirPath + "\\" + name + ".mph";
	}

	// ���� ����
	std::filesystem::path p(DirPath);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);

	// ���� ���� �� ����
	std::ofstream MorphFile;
	MorphFile.open(Path, std::ios::out | std::ios::binary);
	if (!MorphFile.is_open())
	{
		MessageBoxA(0, "Create File Failed", "Warning", MB_OK);
		return false;
	}

	MorphFile << SubmeshCount;

	int VertexSize = 0;
	int SubmeshIDX = 0;
	int VertexIDX = 0;

	for (SubmeshIDX = 0; SubmeshIDX < SubmeshCount; SubmeshIDX++)
	{
		VertexSize = static_cast<int>(target->meshDatas[SubmeshIDX].Vertices.size());

		MorphFile << VertexSize;

		for (VertexIDX = 0; VertexIDX < VertexSize; VertexIDX++)
		{
			MorphFile << target->meshDatas[SubmeshIDX].Vertices[VertexIDX].Position[0];
			MorphFile << target->meshDatas[SubmeshIDX].Vertices[VertexIDX].Position[1];
			MorphFile << target->meshDatas[SubmeshIDX].Vertices[VertexIDX].Position[2];
		}
	}

	MorphFile.close();

	return true;
}

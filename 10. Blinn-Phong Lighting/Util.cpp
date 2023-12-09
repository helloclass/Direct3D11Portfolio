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

// 루트 내의 모든 폴더 패스를 제공
void GetAllSubFolders(std::string root, std::vector<std::string>& Folders)
{
	for (const auto& file : directory_iterator(root))
	{
		Folders.push_back(file.path().string());
	}
}

/**
*
* 애니메이션에 델타 적용은 다음과 같은 수식으로 진행된다.
* 뼈 B0, B1, B2 가 있을 때 B0에 델타가 적용된다. 이때 뎉타가 적용된 B2는?
*
* B2_ = Vn * B2 * (B0)^-1 * B0_delta * B0
*
* 이때 행렬의 곱은 "결합법칙"이 성립된다.
*
* B2_ = Vn * B2 * ((B0)^-1 * B0_delta * B0)
*
* Q> 델타를 곱해야 하는 순서는?
*
*/

// 본의 델타를 업데이트 한다.
// 
// 비용은 그다지 높지 않음.
// 행렬끼리의 곱은 결합법칙이 성립되므로 모든 버텍스에 대해서 새로운 연산을 할 필요가 없음.
// 연산 회수 => 뼈대 개수
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

		// 현재 레벨의 스켈레톤 개수 만큼
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

			// 델타 가중치 매트릭스 저장
			if (Node->Parent)
			{
				Node->Item.DeltaWeightMatrix =
					// ((Bone_Mat)^-1 * Bone_Delta * Bone_Mat)
					inverse(Node->Item.TranslationMatrix) *
					Node->Item.DeltaTranslationMatrix *
					Node->Item.TranslationMatrix *

					// Previous_Bone_Weight_Matrix
					// 자손 뼈대는 부모 뼈대에 종속되어 있음
					Node->Parent->Item.DeltaWeightMatrix; 
			}

			// 다음 차일드 불러오기
			for (Tree<Skeleton>* child : Node->Childs)
				skeletons.push(child);
		}
	}
}

// 뼈대 구조가 똑같으면 true 리턴 다르면 false
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

		// 현재 레벨의 스켈레톤 개수 만큼
		while (qSize--)
		{
			Node = skeletons.front();
			skeletons.pop();

			TargetNode = TargetSkeletons.front();
			TargetSkeletons.pop();

			// 뼈대 구조가 다르다면
			if (Node->Childs.size() != TargetNode->Childs.size())
				return false;

			// 다음 차일드 불러오기
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
	// 우선 서브메쉬 개수 부터 같은지 확인
	if (SubmeshCount != target->SubmeshCount)
		return false;

	// 서브메쉬가 같다면 
	// 서브메쉬 만큼 루프를 돌리며, 버텍스와 인덱스를 검증한다.
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

	// 몽타주에 적용된 시퀸스 개수 알아오기
	UINT SequenceCount = 0;
	MontageFile >> SequenceCount;

	// 시퀀스 개수만큼 클립 사이즈 키우기
	Montages[MontageName].mSequenceList.resize(SequenceCount);

	for (Montage::SequenceClip& clip : Montages[MontageName].mSequenceList)
	{
		// 시퀀스 이름
		MontageFile >> clip.mSequence->mName;
		// 시퀀스 시작 시간
		MontageFile >> clip.mClipStartFrame;
		// 시퀀스 끝 시간
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

	// 블렌드 스페이스에 적용된 시퀸스 개수 및 Weight 알아오기
	UINT SequenceCount = 0;
	float WeightY, WeightX;

	BlendSpaceFile >> SequenceCount;
	BlendSpaceFile >> WeightX >> WeightY;

	// 시퀀스 개수만큼 클립 사이즈 키우기
	BlendSpaces[BlendSpaceName].mClips.resize(SequenceCount);

	for (BlendSpace::SequenceClip& clip : BlendSpaces[BlendSpaceName].mClips)
	{
		// 시퀀스 이름
		BlendSpaceFile >> clip.mSequence->mName;
		// 시퀀스 시작 시간
		BlendSpaceFile >> clip.mPositionY;
		// 시퀀스 끝 시간
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

	// 블렌드 스페이스에 적용된 시퀸스 개수 및 Weight 알아오기
	UINT SequenceCount = 0;
	float WeightY, WeightX;

	CompositeFile >> SequenceCount;
	CompositeFile >> WeightX >> WeightY;

	// 시퀀스 개수만큼 클립 사이즈 키우기
	Composites[CompositeName].mClips.resize(SequenceCount);

	for (Composite::SequenceClip& clip : Composites[CompositeName].mClips)
	{
		// 시퀀스 이름
		CompositeFile >> clip.mSequence->mName;
		// 시퀀스 시작 시간
		CompositeFile >> clip.mPositionX;
		// 시퀀스 끝 시간
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

	// 서브메쉬 개수를 얻음
	MorphFile >> SubmeshCount;
	Morphes[MorphName].mSecondVertex.resize(SubmeshCount);

	for (SubmeshIDX = 0; SubmeshIDX < SubmeshCount; SubmeshIDX++)
	{
		// 버텍스 개수를 얻음
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

	// 폴더 생성
	CreateFolders(DirPath);

	// 파일 오픈 및 생성
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

	// 폴더 생성
	std::filesystem::path p(DirPath);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);

	// 파일 오픈 및 생성
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

		// 몽타주의 시퀀스 개수를 적고
		MontageFile << SequenceCount;

		for (Montage::SequenceClip clip : montage->mSequenceList)
		{
			// 시퀀스 이름
			MontageFile << clip.mSequence->mName;
			// 시퀀스 시작 시간
			MontageFile << clip.mClipStartFrame;
			// 시퀀스 끝 시간
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

	// 폴더 생성
	std::filesystem::path p(DirPath);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);

	// 파일 오픈 및 생성
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

		// 블렌드 스페이스의 시퀀스를 적고
		BlendSpaceFile << SequenceCount;

		// 블렌드 스페이스의 가중치를 적는다.
		BlendSpaceFile << blendspace->mWeightY;
		BlendSpaceFile << blendspace->mWeightX;

		for (BlendSpace::SequenceClip clip : blendspace->mClips)
		{
			// 시퀀스 이름
			BlendSpaceFile << clip.mSequence->mName;
			// 시퀀스 시작 시간
			BlendSpaceFile << clip.mPositionY;
			// 시퀀스 끝 시간
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

	// 폴더 생성
	std::filesystem::path p(DirPath);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);

	// 파일 오픈 및 생성
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

		// 컴포지트의 시퀀스를 적고
		CompositeFile << SequenceCount;

		// 컴포지트의 가중치를 적는다.
		CompositeFile << composite->mWeightY;
		CompositeFile << composite->mWeightX;

		for (Composite::SequenceClip clip : composite->mClips)
		{
			// 시퀀스 이름
			CompositeFile << clip.mSequence->mName;
			// 시퀀스 시작 시간
			CompositeFile << clip.mPositionY;
			// 시퀀스 끝 시간
			CompositeFile << clip.mPositionX;
		}

		CompositeFile.close();
	}

	return true;
}

bool ObjectData::ExportAnimationMorph(ObjectData* target, std::string name)
{
	// 메쉬 검증
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

	// 폴더 생성
	std::filesystem::path p(DirPath);
	if (!std::filesystem::is_directory(p))
		std::filesystem::create_directories(p);

	// 파일 오픈 및 생성
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

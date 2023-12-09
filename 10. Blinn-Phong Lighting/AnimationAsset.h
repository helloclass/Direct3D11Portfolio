#pragma once
#include "GlobalDataStruct.h"

/*
	이후 오브젝트에 애니메이션을 제공할 때
	시퀀스, 몽타주, 블랜드 스페이스를 공통적으로 넣을 수 있다.
	그로인해 AnimationAsset으로 엮어서 관리하는 것이 좋다.
*/
class AnimationAsset
{
public:
	enum AnimationAssetType
	{
		SEQUENCE,
		MONTAGE,
		BLENDSPACE,
		COMPOSITE
	};
public:
	//type

	std::string					mName;
	AnimationAssetType			mType = AnimationAssetType::SEQUENCE;

	UINT						mFrame = 0;
	UINT						mFrameLength = 0;

	// 현 프레임의 애니메이션이 적용된 버텍스 리스트
	// Submesh[Vertex]
	std::vector<std::vector<Vertex>>	mAnimBuffer;

public:
	virtual void GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix) {};
	virtual void GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix) {};

};

// 애니메이션 시퀀스 리스트
class Sequence : public AnimationAsset
{
public:
	Sequence() {
		mType = AnimationAssetType::SEQUENCE;

		mSubmeshSize	= 0;
		mFrameSize		= 0;
		mClusterSize	= 0;
	}

public:
	FbxArray<FbxString*>		mAnimNameLists;

	UINT						mSubmeshSize;
	UINT						mFrameSize;
	UINT						mClusterSize;

	std::vector<FbxTime>		mStarts;
	std::vector<FbxTime>		mStops;

	// [Submesh][Frame][Cluster][Matrix]
	float**** mAnimVertexArrays = nullptr;
	FbxUInt* mAnimVertexSizes = nullptr;

public:
	void GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix) override;
	void GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix) override;
};

class Montage : public AnimationAsset
{
public:
	Montage() {
		mType = AnimationAssetType::MONTAGE;
	}

public:
	bool mIsBlend = false;
	UINT mBlendFrame = 50;

	UINT mCurrentSequenceIndex = 0;

	struct SequenceClip
	{
		Sequence* mSequence;
		UINT mClipStartFrame;
		UINT mClipEndFrame;
	};

	std::list<SequenceClip> mSequenceList;

public:
	void GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix) override;
	void GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix) override;

public:
	inline void PushSequence(Sequence* newSequence)
	{
		if (!newSequence)
			return;

		SequenceClip newClip;

		if (0 < mSequenceList.size())
		{
			newClip.mSequence = newSequence;
			newClip.mClipStartFrame = mFrameLength - mBlendFrame;
			newClip.mClipEndFrame = newSequence->mFrameLength + mFrameLength - mBlendFrame;
		}
		else
		{
			newClip.mSequence = newSequence;
			newClip.mClipStartFrame = mFrameLength;
			newClip.mClipEndFrame = newSequence->mFrameLength + mFrameLength;
		}

		mSequenceList.push_back(newClip);

		// 애니메이션 프레임 누적
		mFrameLength = newClip.mClipEndFrame;
	}

	inline void PopSequence()
	{
		if (0 < mSequenceList.size())
		{
			Montage::SequenceClip& clip = *(--mSequenceList.end());
			mFrameLength -= (clip.mClipEndFrame - clip.mClipStartFrame);
			mSequenceList.erase(--(mSequenceList.end()));

			return;
		}
	}
};

/*
	정해진 가중치에 따라 두 애니메이션을 섞는다.

	ex> 킥이나 펀치 망치를 내려치는 등의 모션에 원하는 위치로 '저격'을 하여 날리는 것이 가능
	ex> IDLE과 Run애니메이션을 속도로 보간시켜 자연스러운 움직임이 가능
*/
class BlendSpace : public AnimationAsset
{
public:
	BlendSpace() {
		mType = AnimationAssetType::BLENDSPACE;
	}

public:
	float			mWeightY = 0.0f;
	float			mWeightX = 0.0f;

	// 몽타주도 넣는게 좋을까?
	struct SequenceClip
	{
		Sequence* mSequence;
		float mPositionY;
		float mPositionX;
	};

	std::vector<SequenceClip> mClips;

public:
	void GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix) override;
	void GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix) override;

public:
	inline void PushSequence(Sequence* newSequence, float& posX, float& posY)
	{
		if (!newSequence)
			return;

		SequenceClip newClip;

		newClip.mSequence = newSequence;
		newClip.mPositionY = 0.0f;
		newClip.mPositionX = 0.0f;

		mClips.push_back(newClip);

		// 가장 긴 애니메이션을 기준으로 잡기에 최대 애니메이션 길이를 잡는다.
		mFrameLength = (mFrameLength < newSequence->mFrameLength) ?
			newSequence->mFrameLength : mFrameLength;
	}

	inline void PopSequence()
	{
		if (0 < mClips.size())
		{
			mClips.erase(--(mClips.end()));
			mFrameLength = 0;

			// 최대 길이 애니메이션 수정
			for (BlendSpace::SequenceClip clip : mClips)
			{
				mFrameLength = (mFrameLength < clip.mSequence->mFrameLength) ?
					clip.mSequence->mFrameLength : mFrameLength;
			}

			return;
		}
	}
};

/*
	정해진 가중치에 따라 두 애니메이션을 섞는다.
	단, 특정 뼈대 부위를 기점으로 그 뼈대의 상위 계층만 블렌딩 시키고 나머지는 Default 애니메이션을 따른다.

	ex> 달리기, 펀치 애니메이션을 하체 따로 상체 따로 합쳐 달리면서 펀치하는 애니메이션 생성 가능
	ex> LookAt 기능과 혼합하여 Aim Offset 제작 가능
*/
class Composite : public AnimationAsset
{
public:
	Composite() {
		mType = AnimationAssetType::COMPOSITE;
	}

public:
	/*
		컴포지트의 특징으로, 지정된 뼈대의 상위 계층은 블렌딩 시키고
		뼈대의 하위 계층은 블렌딩 시키지 않는다.
		이를 지시하기 위해 다음 변수를 생성시킴.
		(GetAnimationMatrix는 override 상태라 bool 파라미터를 추가적으로 받기 힘든 상태)
	*/
	bool			mIsBlending = false;

	float			mWeightY = 0.0f;
	float			mWeightX = 0.0f;

	std::string		mPivotBoneName;

	struct SequenceClip
	{
		Sequence* mSequence;
		float mPositionY;
		float mPositionX;
	};

	std::vector<SequenceClip> mClips;

public:
	void GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix) override;
	void GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix) override;

public:
	inline void PushSequence(Sequence* newSequence, float& posY, float& posX)
	{
		if (!newSequence)
			return;

		SequenceClip newClip;

		newClip.mSequence = newSequence;
		newClip.mPositionY = posY;
		newClip.mPositionX = posX;

		mClips.push_back(newClip);

		// 가장 긴 애니메이션을 기준으로 잡기에 최대 애니메이션 길이를 잡는다.
		mFrameLength = (mFrameLength < newSequence->mFrameLength) ?
			newSequence->mFrameLength : mFrameLength;
	}

	inline void PopSequence()
	{
		if (0 < mClips.size())
		{
			mClips.erase(--(mClips.end()));
			mFrameLength = 0;

			// 최대 길이 애니메이션 수정
			for (Composite::SequenceClip clip : mClips)
			{
				mFrameLength = (mFrameLength < clip.mSequence->mFrameLength) ?
					clip.mSequence->mFrameLength : mFrameLength;
			}

			return;
		}
	}
};

// Create Morph, Edit Morph, Adapted Morph 시
// Target Data가 필요함.
// 전역 변수로 만들어볼 필요가 있음.
struct Morph
{
public:
	std::string			mName;

	float				mWeight;
	float				mMaxWeight;

	// [Submesh][Vertex-Index]
	std::vector<std::vector<float3>> mSecondVertex;
};
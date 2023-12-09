#pragma once
#include "GlobalDataStruct.h"

/*
	���� ������Ʈ�� �ִϸ��̼��� ������ ��
	������, ��Ÿ��, ���� �����̽��� ���������� ���� �� �ִ�.
	�׷����� AnimationAsset���� ��� �����ϴ� ���� ����.
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

	// �� �������� �ִϸ��̼��� ����� ���ؽ� ����Ʈ
	// Submesh[Vertex]
	std::vector<std::vector<Vertex>>	mAnimBuffer;

public:
	virtual void GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix) {};
	virtual void GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix) {};

};

// �ִϸ��̼� ������ ����Ʈ
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

		// �ִϸ��̼� ������ ����
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
	������ ����ġ�� ���� �� �ִϸ��̼��� ���´�.

	ex> ű�̳� ��ġ ��ġ�� ����ġ�� ���� ��ǿ� ���ϴ� ��ġ�� '����'�� �Ͽ� ������ ���� ����
	ex> IDLE�� Run�ִϸ��̼��� �ӵ��� �������� �ڿ������� �������� ����
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

	// ��Ÿ�ֵ� �ִ°� ������?
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

		// ���� �� �ִϸ��̼��� �������� ��⿡ �ִ� �ִϸ��̼� ���̸� ��´�.
		mFrameLength = (mFrameLength < newSequence->mFrameLength) ?
			newSequence->mFrameLength : mFrameLength;
	}

	inline void PopSequence()
	{
		if (0 < mClips.size())
		{
			mClips.erase(--(mClips.end()));
			mFrameLength = 0;

			// �ִ� ���� �ִϸ��̼� ����
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
	������ ����ġ�� ���� �� �ִϸ��̼��� ���´�.
	��, Ư�� ���� ������ �������� �� ������ ���� ������ ���� ��Ű�� �������� Default �ִϸ��̼��� ������.

	ex> �޸���, ��ġ �ִϸ��̼��� ��ü ���� ��ü ���� ���� �޸��鼭 ��ġ�ϴ� �ִϸ��̼� ���� ����
	ex> LookAt ��ɰ� ȥ���Ͽ� Aim Offset ���� ����
*/
class Composite : public AnimationAsset
{
public:
	Composite() {
		mType = AnimationAssetType::COMPOSITE;
	}

public:
	/*
		������Ʈ�� Ư¡����, ������ ������ ���� ������ ���� ��Ű��
		������ ���� ������ ���� ��Ű�� �ʴ´�.
		�̸� �����ϱ� ���� ���� ������ ������Ŵ.
		(GetAnimationMatrix�� override ���¶� bool �Ķ���͸� �߰������� �ޱ� ���� ����)
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

		// ���� �� �ִϸ��̼��� �������� ��⿡ �ִ� �ִϸ��̼� ���̸� ��´�.
		mFrameLength = (mFrameLength < newSequence->mFrameLength) ?
			newSequence->mFrameLength : mFrameLength;
	}

	inline void PopSequence()
	{
		if (0 < mClips.size())
		{
			mClips.erase(--(mClips.end()));
			mFrameLength = 0;

			// �ִ� ���� �ִϸ��̼� ����
			for (Composite::SequenceClip clip : mClips)
			{
				mFrameLength = (mFrameLength < clip.mSequence->mFrameLength) ?
					clip.mSequence->mFrameLength : mFrameLength;
			}

			return;
		}
	}
};

// Create Morph, Edit Morph, Adapted Morph ��
// Target Data�� �ʿ���.
// ���� ������ ���� �ʿ䰡 ����.
struct Morph
{
public:
	std::string			mName;

	float				mWeight;
	float				mMaxWeight;

	// [Submesh][Vertex-Index]
	std::vector<std::vector<float3>> mSecondVertex;
};
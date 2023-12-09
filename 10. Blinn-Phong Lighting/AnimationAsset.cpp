#include "AnimationAsset.h"

void Sequence::GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix)
{
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			matrix.m[i][j] = mAnimVertexArrays[SubmeshIDX][mFrame][ClusterIDX][i * 4 + j];
}
void Sequence::GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix)
{
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			matrix.mData[i].mData[j] = mAnimVertexArrays[SubmeshIDX][mFrame][ClusterIDX][i * 4 + j];
}

void Montage::GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix)
{
	int i, j;
	float multiply = 0.0f;
	matrix = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	UINT sequenceIDX = 0;
	float weight = 1.0f;
	for (Montage::SequenceClip clip : mSequenceList)
	{
		if (clip.mClipStartFrame <= mFrame && mFrame < clip.mClipEndFrame)
		{
			weight = 1.0f;

			if (0 < sequenceIDX && mFrame <= clip.mClipStartFrame + mBlendFrame)
			{
				weight =
					(clip.mClipStartFrame + mBlendFrame - mFrame) /
					(float)mBlendFrame;
				weight = 1.0f - weight;
			}
			else if (sequenceIDX < mSequenceList.size() - 1 && clip.mClipEndFrame - mBlendFrame <= mFrame)
			{
				weight =
					(mFrame - (clip.mClipEndFrame - mBlendFrame)) /
					(float)mBlendFrame;
				weight = 1.0f - weight;
			}

			multiply += 1.0f;
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					matrix.m[i][j] +=
					(clip.mSequence->mAnimVertexArrays[SubmeshIDX][mFrame - clip.mClipStartFrame][ClusterIDX][i * 4 + j]) * weight;
		}

		sequenceIDX++;
	}
}
void Montage::GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix)
{
	int i, j;
	float multiply = 0.0f;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			matrix.mData[i].mData[j] = 0.0f;

	UINT sequenceIDX = 0;
	float weight = 1.0f;
	for (Montage::SequenceClip clip : mSequenceList)
	{
		if (clip.mClipStartFrame <= mFrame && mFrame < clip.mClipEndFrame)
		{
			weight = 1.0f;

			if (0 < sequenceIDX && mFrame <= clip.mClipStartFrame + mBlendFrame)
			{
				weight =
					(clip.mClipStartFrame + mBlendFrame - mFrame) /
					(float)mBlendFrame;
				weight = 1.0f - weight;
			}
			else if (sequenceIDX < mSequenceList.size() - 1 && clip.mClipEndFrame - mBlendFrame <= mFrame)
			{
				weight =
					(mFrame - (clip.mClipEndFrame - mBlendFrame)) /
					(float)mBlendFrame;
				weight = 1.0f - weight;
			}

			multiply += 1.0f;
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					matrix.mData[i].mData[j] +=
					(clip.mSequence->mAnimVertexArrays[SubmeshIDX][mFrame - clip.mClipStartFrame][ClusterIDX][i * 4 + j]) * weight;
		}

		sequenceIDX++;
	}
}

void BlendSpace::GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix)
{
	int i, j;
	float multiply = 0.0f;
	matrix = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	UINT sequenceIDX = 0;

	float totalDist = 0.0f;
	std::vector<float> weight(mClips.size(), 0.0f);

	for (BlendSpace::SequenceClip& clip : mClips)
	{
		weight[sequenceIDX] = pow(clip.mPositionY - mWeightY, 2.0) + pow(clip.mPositionX - mWeightX, 2.0);
		totalDist += weight[sequenceIDX];

		sequenceIDX++;
	}

	sequenceIDX = 0;
	for (BlendSpace::SequenceClip& clip : mClips)
	{
		// 가중치 값을 Normalization
		weight[sequenceIDX] /= totalDist;

		// 각 클립은 가장 긴 시퀀스가 끝날 때 까지 계속 반복한다 (나머지 연산)
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				matrix.m[i][j] +=
				(clip.mSequence->mAnimVertexArrays[SubmeshIDX][mFrame % clip.mSequence->mFrameLength][ClusterIDX][i * 4 + j]) * weight[sequenceIDX];

		sequenceIDX++;
	}
}
void BlendSpace::GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix)
{
	int i, j;
	float multiply = 0.0f;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			matrix.mData[i].mData[j] = 0.0f;

	UINT sequenceIDX = 0;

	float totalDist = 0.0f;
	std::vector<float> weight(mClips.size(), 0.0f);

	for (BlendSpace::SequenceClip& clip : mClips)
	{
		weight[sequenceIDX] = pow(clip.mPositionY - mWeightY, 2.0) + pow(clip.mPositionX - mWeightX, 2.0);
		totalDist += weight[sequenceIDX];

		sequenceIDX++;
	}

	sequenceIDX = 0;
	for (BlendSpace::SequenceClip& clip : mClips)
	{
		// 가중치 값을 Normalization
		weight[sequenceIDX] /= totalDist;

		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				matrix.mData[i].mData[j] +=
				(clip.mSequence->mAnimVertexArrays[SubmeshIDX][mFrame % clip.mSequence->mFrameLength][ClusterIDX][i * 4 + j]) * weight[sequenceIDX];

		sequenceIDX++;
	}
}

void Composite::GetAnimationMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, float4x4& matrix)
{
	int i, j;
	float multiply = 0.0f;
	matrix = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	// 지정 포인트 뼈대이거나 혹은 그 상위의 뼈대일 경우
	if (this->mIsBlending)
	{
		UINT sequenceIDX = 0;

		float totalDist = 0.0f;
		std::vector<float> weight(mClips.size(), 0.0f);

		for (Composite::SequenceClip& clip : mClips)
		{
			weight[sequenceIDX] = pow(clip.mPositionY - mWeightY, 2.0) + pow(clip.mPositionX - mWeightX, 2.0);
			totalDist += weight[sequenceIDX];

			sequenceIDX++;
		}

		sequenceIDX = 0;
		for (Composite::SequenceClip& clip : mClips)
		{
			// 가중치 값을 Normalization
			weight[sequenceIDX] /= totalDist;

			// 각 클립은 가장 긴 시퀀스가 끝날 때 까지 계속 반복한다 (나머지 연산)
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					matrix.m[i][j] +=
					(clip.mSequence->mAnimVertexArrays[SubmeshIDX][mFrame % clip.mSequence->mFrameLength][ClusterIDX][i * 4 + j]) * weight[sequenceIDX];

			sequenceIDX++;
		}
	}
	// 지정 포인트 뼈대의 하위 뼈대일 경우
	else
	{
		for (Composite::SequenceClip& clip : mClips)
		{
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					matrix.m[i][j] +=
					(clip.mSequence->mAnimVertexArrays[SubmeshIDX][mFrame % clip.mSequence->mFrameLength][ClusterIDX][i * 4 + j]);

			// 100% Play Clip Zero
			break;
		}
	}
}
void Composite::GetAnimationFBXMatrix(UINT& SubmeshIDX, UINT& ClusterIDX, fbxsdk::FbxAMatrix& matrix)
{
	int i, j;
	float multiply = 0.0f;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			matrix.mData[i].mData[j] = 0.0f;

	// 지정 포인트 뼈대이거나 혹은 그 상위의 뼈대일 경우
	if (this->mIsBlending)
	{
		UINT sequenceIDX = 0;

		float totalDist = 0.0f;
		std::vector<float> weight(mClips.size(), 0.0f);

		for (Composite::SequenceClip& clip : mClips)
		{
			weight[sequenceIDX] = pow(clip.mPositionY - mWeightY, 2.0) + pow(clip.mPositionX - mWeightX, 2.0);
			totalDist += weight[sequenceIDX];

			sequenceIDX++;
		}

		sequenceIDX = 0;
		for (Composite::SequenceClip& clip : mClips)
		{
			// 가중치 값을 Normalization
			weight[sequenceIDX] /= totalDist;

			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					matrix.mData[i].mData[j] +=
					(clip.mSequence->mAnimVertexArrays[SubmeshIDX][mFrame % clip.mSequence->mFrameLength][ClusterIDX][i * 4 + j]) * weight[sequenceIDX];

			sequenceIDX++;
		}
	}
	// 지정 포인트 뼈대의 하위 뼈대일 경우
	else
	{
		for (Composite::SequenceClip& clip : mClips)
		{
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					matrix.mData[i].mData[j] +=
					(clip.mSequence->mAnimVertexArrays[SubmeshIDX][mFrame % clip.mSequence->mFrameLength][ClusterIDX][i * 4 + j]);

			// 100% Play Clip Zero
			break;
		}
	}
}

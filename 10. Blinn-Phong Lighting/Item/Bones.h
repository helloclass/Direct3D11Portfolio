#pragma once
#include "../DirectX.h"

class Bone : public UIObjectData
{
private:
	std::string ObjectName = "Bone";
	std::string ObjectPath = "Resource\\Skeletal_Mesh.fbx";

	std::string TargetInstanceName;
	DynamicObjectData* Target = nullptr;

	std::unordered_map<std::string, float4x4> Bones;
	std::vector<int> Indices;
	UINT BoneSize;

public:
	Bone() {
		fbxLoader.CreateFBXModel(
			this,
			ObjectName,
			ObjectPath
		);

		ShaderID = "Axis.hlsl";
		bIsEnableDepthStencil = false;
	}

	void SetTarget(DynamicObjectData* data, std::string InstanceName)
	{
		Target = data;
		TargetInstanceName = InstanceName;

		Target->GetAllBones(Bones);
		BoneSize = static_cast<UINT>(Bones.size());

		for (std::pair<std::string, float4x4> bone : Bones)
		{
			Instantiate(bone.first);
		}
	}

	DynamicObjectData* GetTarget()
	{
		return Target;
	}

	void Awake() {
		UIObjectData::Awake();
	}

	void Start() {
		UIObjectData::Start();

	}

	void Update() {
		UIObjectData::Update();
		float4x4 WorldMat;

		{
			UINT submeshIDX = 0;
			UINT currentFrame = 0;

			FbxSkin* lSkinDeformer;

			UINT lClusterCount = 0;
			UINT lClusterIndex = 0;
			FbxCluster* lCluster;

			std::string name;
			int i(0), j(0), k(0);

			fbxsdk::FbxVector4* vertices;
			fbxsdk::FbxVector4	result;

			fbxsdk::FbxAMatrix	matrix;
			float4x4			localMatrix;

			Target->transform->InstanceBuffer[TargetInstanceName]->GetAffineMatrix(localMatrix);

			int* mIndices = nullptr;

			Tree<Skeleton>* bone = nullptr;

			float**** AnimVertexArray = nullptr;

			if (Target->CurrentAsset->mType == AnimationAsset::AnimationAssetType::SEQUENCE)
			{
				Sequence* asset = (Sequence*)Target->GetAsset();

				// 애니메이션을 업데이트 한다.
				currentFrame = asset->mFrame;

				for (submeshIDX = 0; submeshIDX < SubmeshCount; submeshIDX++)
				{
					vertices = Target->meshFBXDatas[submeshIDX]->GetControlPoints();

					lSkinDeformer =
						(FbxSkin*)Target->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

					lClusterCount =
						lSkinDeformer->GetClusterCount();

					for (lClusterIndex = 0; lClusterIndex < lClusterCount; lClusterIndex++)
					{
						lCluster = lSkinDeformer->GetCluster(lClusterIndex);
						mIndices = lCluster->GetControlPointIndices();

						if (!mIndices)
							continue;

						name = lCluster->GetNameOnly();

						bone = Target->BoneHierarchy.Find(name);

						asset->GetAnimationMatrix(submeshIDX, lClusterIndex, bone->Item.TranslationMatrix);
						asset->GetAnimationFBXMatrix(submeshIDX, lClusterIndex, matrix);

						// Adjust Bone Position
						result = matrix.MultT(vertices[mIndices[0]]);
						// Adjust Local, Global Position that Object is exist
						result = localMatrix.toFbxAMatrix().MultT(result);

						bone->Item.Position.x = static_cast<float>(result.mData[0]);
						bone->Item.Position.y = static_cast<float>(result.mData[1]);
						bone->Item.Position.z = static_cast<float>(result.mData[2]);
						bone->Item.Position.w = static_cast<float>(result.mData[3]);

						// Adjust Bone Rotation
						result = matrix.MultR(vertices[mIndices[0]]);
						// Adjust Local, Global Rotation
						result = localMatrix.toFbxAMatrix().MultR(result);

						bone->Item.Rotation.x = static_cast<float>(result.mData[0]);
						bone->Item.Rotation.y = static_cast<float>(result.mData[1]);
						bone->Item.Rotation.z = static_cast<float>(result.mData[2]);
						bone->Item.Rotation.w = static_cast<float>(result.mData[3]);

					} // ClusterCount
				} // SubmeshCount
			}
			if (Target->CurrentAsset->mType == AnimationAsset::AnimationAssetType::MONTAGE)
			{
				Montage* asset = (Montage*)Target->GetAsset();

				// 애니메이션을 업데이트 한다.
				currentFrame = asset->mFrame;

				for (submeshIDX = 0; submeshIDX < SubmeshCount; submeshIDX++)
				{
					vertices = Target->meshFBXDatas[submeshIDX]->GetControlPoints();

					lSkinDeformer =
						(FbxSkin*)Target->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

					lClusterCount =
						lSkinDeformer->GetClusterCount();

					for (lClusterIndex = 0; lClusterIndex < lClusterCount; lClusterIndex++)
					{
						lCluster = lSkinDeformer->GetCluster(lClusterIndex);
						mIndices = lCluster->GetControlPointIndices();

						if (!mIndices)
							continue;

						name = lCluster->GetNameOnly();

						bone = Target->BoneHierarchy.Find(name);

						asset->GetAnimationMatrix(submeshIDX, lClusterIndex, bone->Item.TranslationMatrix);
						asset->GetAnimationFBXMatrix(submeshIDX, lClusterIndex, matrix);

						// Adjust Bone Position
						result = matrix.MultT(vertices[mIndices[0]]);
						// Adjust Local, Global Position that Object is exist
						result = localMatrix.toFbxAMatrix().MultT(result);

						bone->Item.Position.x = static_cast<float>(result.mData[0]);
						bone->Item.Position.y = static_cast<float>(result.mData[1]);
						bone->Item.Position.z = static_cast<float>(result.mData[2]);
						bone->Item.Position.w = static_cast<float>(result.mData[3]);

						// Adjust Bone Rotation
						result = matrix.MultR(vertices[mIndices[0]]);
						// Adjust Local, Global Rotation
						result = localMatrix.toFbxAMatrix().MultR(result);

						bone->Item.Rotation.x = static_cast<float>(result.mData[0]);
						bone->Item.Rotation.y = static_cast<float>(result.mData[1]);
						bone->Item.Rotation.z = static_cast<float>(result.mData[2]);
						bone->Item.Rotation.w = static_cast<float>(result.mData[3]);

					} // ClusterCount
				} // SubmeshCount
			}
			if (Target->CurrentAsset->mType == AnimationAsset::AnimationAssetType::BLENDSPACE)
			{
				BlendSpace* asset = (BlendSpace*)Target->GetAsset();

				// 애니메이션을 업데이트 한다.
				currentFrame = asset->mFrame;

				for (submeshIDX = 0; submeshIDX < SubmeshCount; submeshIDX++)
				{
					vertices = Target->meshFBXDatas[submeshIDX]->GetControlPoints();

					lSkinDeformer =
						(FbxSkin*)Target->meshFBXDatas[submeshIDX]->GetDeformer(0, FbxDeformer::eSkin);

					lClusterCount =
						lSkinDeformer->GetClusterCount();

					for (lClusterIndex = 0; lClusterIndex < lClusterCount; lClusterIndex++)
					{
						lCluster = lSkinDeformer->GetCluster(lClusterIndex);
						mIndices = lCluster->GetControlPointIndices();

						if (!mIndices)
							continue;

						name = lCluster->GetNameOnly();

						bone = Target->BoneHierarchy.Find(name);

						asset->GetAnimationMatrix(submeshIDX, lClusterIndex, bone->Item.TranslationMatrix);
						asset->GetAnimationFBXMatrix(submeshIDX, lClusterIndex, matrix);

						// Adjust Bone Position
						result = matrix.MultT(vertices[mIndices[0]]);
						// Adjust Local, Global Position that Object is exist
						result = localMatrix.toFbxAMatrix().MultT(result);

						bone->Item.Position.x = static_cast<float>(result.mData[0]);
						bone->Item.Position.y = static_cast<float>(result.mData[1]);
						bone->Item.Position.z = static_cast<float>(result.mData[2]);
						bone->Item.Position.w = static_cast<float>(result.mData[3]);

						// Adjust Bone Rotation
						result = matrix.MultR(vertices[mIndices[0]]);
						// Adjust Local, Global Rotation
						result = localMatrix.toFbxAMatrix().MultR(result);

						bone->Item.Rotation.x = static_cast<float>(result.mData[0]);
						bone->Item.Rotation.y = static_cast<float>(result.mData[1]);
						bone->Item.Rotation.z = static_cast<float>(result.mData[2]);
						bone->Item.Rotation.w = static_cast<float>(result.mData[3]);

					} // ClusterCount
				} // SubmeshCount
			}
		}

		if (Target)
		{
			float4 delta;
			std::vector<Tree<Skeleton>*> TargetBone;
			Target->BoneHierarchy.GetAllChilds(
				(Tree<Skeleton>*)Target->BoneHierarchy.Root, 
				TargetBone
			);

			for (Tree<Skeleton>* bone : TargetBone)
			{
				if (!transform->InstanceBuffer[bone->Name])
				{
					transform->InstanceBuffer[bone->Name] = new Transform::transform();
				}

				transform->InstanceBuffer[bone->Name]->SetTransform(
					bone->Item.Position.x, bone->Item.Position.y, bone->Item.Position.z,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f
				);

				// Parent Bone으로 Bone을 늘림
				if (bone->Parent)
				{
					delta = bone->Parent->Item.Position - bone->Item.Position;

					Target->collider.Extents.x = delta.x;
					Target->collider.Extents.y = delta.y;
					Target->collider.Extents.z = delta.z;
				}
			}
		}
	}

	void Destroy() {
		UIObjectData::Destroy();

	}
};
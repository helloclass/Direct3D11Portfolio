#pragma once

#include "PhysicsObjectData.h"

namespace oguna
{
	class EncodingConverter {
	protected:
		std::vector<char> buffer;

	public:
		EncodingConverter(int initial_buffer_size = MAX_PATH)
		{
			buffer.resize(initial_buffer_size);
		}

		int Utf8ToCp932(const char* src, int size, std::string* out)
		{
			std::wstring unicode;
			Utf8ToUtf16(src, size, &unicode);
			return Utf16ToCp932(unicode.data(), (int)unicode.length(), out);
		}

		int Cp932ToUtf8(const char* src, int length, std::string* out)
		{
			std::wstring unicode;
			Cp932ToUtf16(src, length, &unicode);
			return Utf16ToUtf8(unicode.c_str(), (int)unicode.length(), out);
		}

		int Cp932ToUtf16(const char* src, int length, std::wstring* out)
		{
			int size;
			size = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, length, NULL, NULL);
			buffer.resize(size * sizeof(wchar_t) * 2);
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, length, (LPWSTR)buffer.data(), (int)buffer.size() * 2);
			out->assign((wchar_t*)buffer.data(), size);
			return size;
		}

		int Utf16ToCp932(const wchar_t* src, int length, std::string* out)
		{
			int size;
			size = WideCharToMultiByte(CP_ACP, NULL, src, length, NULL, NULL, NULL, NULL);
			buffer.resize(size);
			WideCharToMultiByte(CP_ACP, NULL, src, length, (LPSTR)buffer.data(), (int)buffer.size(), NULL, NULL);
			out->assign(buffer.data(), size);
			return size;
		}

		int Utf8ToUtf16(const char* src, int length, std::wstring* out)
		{
			int size;
			size = ::MultiByteToWideChar(CP_UTF8, 0, src, length, NULL, NULL);
			buffer.resize(size * sizeof(wchar_t));
			MultiByteToWideChar(CP_UTF8, 0, src, length, (LPWSTR)buffer.data(), (int)buffer.size());

			std::wstring data = std::wstring((wchar_t*)buffer.data(), size);
			out->swap(data);
			return size;
		}

		int Utf16ToUtf8(const wchar_t* src, int length, std::string* out)
		{
			int size;
			size = WideCharToMultiByte(CP_UTF8, NULL, src, length, NULL, NULL, NULL, NULL);
			buffer.resize(size);
			WideCharToMultiByte(CP_UTF8, NULL, src, length, (LPSTR)buffer.data(), (int)buffer.size(), NULL, NULL);
			out->assign(buffer.data(), size);
			return size;
		}
	};
}

namespace geo
{
	int CreateBox(
		std::string Name,
		float width,
		float height,
		float depth
	);

	int CreateSphere(
		std::string Name,
		float radius,
		uint32 sliceCount,
		uint32 stackCount
	);
}

namespace obj
{
	struct LoadedObj
	{
		uint32_t numVertices;
		uint32_t numIndices;

		Vertex*   vertexBuffer;
		uint16_t* indexBuffer;
	};

	LoadedObj loadObj(const char* filename);
	void freeLoadedObj(LoadedObj loadedObj);
}

namespace pmx
{
#ifndef __unix__
#define utfstring std::wstring
#else
#define utfstring std::string
#endif
	// PMX Loader
	ObjectData* CreatePMXModel(
		_Out_ ObjectData* newData,
		_In_ std::string Name,
		_In_ std::string Path
	);

	class PmxSetting
	{
	public:
		uint8_t encoding;
		uint8_t uv;
		uint8_t vertex_index_size;
		uint8_t texture_index_size;
		uint8_t material_index_size;
		uint8_t bone_index_size;
		uint8_t morph_index_size;
		uint8_t rigidbody_index_size;
	public:
		PmxSetting()
			: encoding(0)
			, uv(0)
			, vertex_index_size(0)
			, texture_index_size(0)
			, material_index_size(0)
			, bone_index_size(0)
			, morph_index_size(0)
			, rigidbody_index_size(0)
		{}

		void Read(std::istream* stream);
	};

	enum class PmxVertexSkinningType : uint8_t
	{
		BDEF1 = 0,
		BDEF2 = 1,
		BDEF4 = 2,
		SDEF = 3,
		QDEF = 4,
	};

	class PmxVertexSkinning
	{
	public:
		virtual void Read(std::istream* stream, PmxSetting* setting) = 0;
	};

	class PmxVertexSkinningBDEF1 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF1()
			: bone_index(0)
		{}

		int bone_index;
		void Read(std::istream* stresam, PmxSetting* setting);
	};

	class PmxVertexSkinningBDEF2 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF2()
			: bone_index1(0)
			, bone_index2(0)
			, bone_weight(0.0f)
		{}

		int bone_index1;
		int bone_index2;
		float bone_weight;
		void Read(std::istream* stresam, PmxSetting* setting);
	};

	class PmxVertexSkinningBDEF4 : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningBDEF4()
			: bone_index1(0)
			, bone_index2(0)
			, bone_index3(0)
			, bone_index4(0)
			, bone_weight1(0.0f)
			, bone_weight2(0.0f)
			, bone_weight3(0.0f)
			, bone_weight4(0.0f)
		{}

		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;
		void Read(std::istream* stresam, PmxSetting* setting);
	};

	class PmxVertexSkinningSDEF : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningSDEF()
			: bone_index1(0)
			, bone_index2(0)
			, bone_weight(0.0f)
		{
			for (int i = 0; i < 3; ++i) {
				sdef_c[i] = 0.0f;
				sdef_r0[i] = 0.0f;
				sdef_r1[i] = 0.0f;
			}
		}

		int bone_index1;
		int bone_index2;
		float bone_weight;
		float sdef_c[3];
		float sdef_r0[3];
		float sdef_r1[3];
		void Read(std::istream* stresam, PmxSetting* setting);
	};

	class PmxVertexSkinningQDEF : public PmxVertexSkinning
	{
	public:
		PmxVertexSkinningQDEF()
			: bone_index1(0)
			, bone_index2(0)
			, bone_index3(0)
			, bone_index4(0)
			, bone_weight1(0.0f)
			, bone_weight2(0.0f)
			, bone_weight3(0.0f)
			, bone_weight4(0.0f)
		{}

		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;
		void Read(std::istream* stresam, PmxSetting* setting);
	};

	class PmxVertex
	{
	public:
		PmxVertex() : 
			edge(0.0f),
			skinning_type(pmx::PmxVertexSkinningType::BDEF1)
		{
			uv[0] = uv[1] = 0.0f;
			for (int i = 0; i < 3; ++i) {
				position[i] = 0.0f;
				normal[i] = 0.0f;
				tangent[i] = 0.0f;
			}
			for (int i = 0; i < 4; ++i) {
				for (int k = 0; k < 4; ++k) {
					uva[i][k] = 0.0f;
				}
			}
		}

		float position[3];
		float normal[3];
		float tangent[3];
		float uv[2];
		float uva[4][4];
		PmxVertexSkinningType skinning_type;
		std::unique_ptr<PmxVertexSkinning> skinning;
		float edge;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	class PmxMaterial
	{
	public:
		PmxMaterial()
			: specularlity(0.0f)
			, flag(0)
			, edge_size(0.0f)
			, diffuse_texture_index(0)
			, sphere_texture_index(0)
			, sphere_op_mode(0)
			, common_toon_flag(0)
			, toon_texture_index(0)
			, index_count(0)
		{
			for (int i = 0; i < 3; ++i) {
				specular[i] = 0.0f;
				ambient[i] = 0.0f;
				edge_color[i] = 0.0f;
			}
			for (int i = 0; i < 4; ++i) {
				diffuse[i] = 0.0f;
			}
		}

		utfstring material_name;
		utfstring material_english_name;
		float diffuse[4];
		float specular[3];
		float specularlity;
		float ambient[3];
		uint8_t flag;
		float edge_color[4];
		float edge_size;
		int diffuse_texture_index;
		int sphere_texture_index;
		uint8_t sphere_op_mode;
		uint8_t common_toon_flag;
		int toon_texture_index;
		utfstring memo;
		int index_count;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	class PmxIkLink
	{
	public:
		PmxIkLink()
			: link_target(0)
			, angle_lock(0)
		{
			for (int i = 0; i < 3; ++i) {
				max_radian[i] = 0.0f;
				min_radian[i] = 0.0f;
			}
		}

		int link_target;
		uint8_t angle_lock;
		float max_radian[3];
		float min_radian[3];
		void Read(std::istream* stream, PmxSetting* settingn);
	};

	class PmxBone
	{
	public:
		PmxBone()
			: parent_index(0)
			, level(0)
			, bone_flag(0)
			, target_index(0)
			, grant_parent_index(0)
			, grant_weight(0.0f)
			, key(0)
			, ik_target_bone_index(0)
			, ik_loop(0)
			, ik_loop_angle_limit(0.0f)
			, ik_link_count(0)
			, localMatrix()
		{
			for (int i = 0; i < 3; ++i) {
				position[i] = 0.0f;
				offset[i] = 0.0f;
				lock_axis_orientation[i] = 0.0f;
				local_axis_x_orientation[i] = 0.0f;
				local_axis_y_orientation[i] = 0.0f;
				local_axis_z_orientation[i] = 0.0f;
			}
		}

		utfstring bone_name;
		utfstring bone_english_name;
		float position[3];
		int parent_index;
		int level;
		uint16_t bone_flag;
		float offset[3];
		int target_index;
		int grant_parent_index;
		float grant_weight;
		float lock_axis_orientation[3];
		float local_axis_x_orientation[3];
		float local_axis_y_orientation[3];
		float local_axis_z_orientation[3];
		float localMatrix[4][4];
		int key;
		int ik_target_bone_index;
		int ik_loop;
		float ik_loop_angle_limit;
		int ik_link_count;
		std::unique_ptr<PmxIkLink[]> ik_links;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	enum class MorphType : uint8_t
	{
		Group = 0,
		Vertex = 1,
		Bone = 2,
		UV = 3,
		AdditionalUV1 = 4,
		AdditionalUV2 = 5,
		AdditionalUV3 = 6,
		AdditionalUV4 = 7,
		Matrial = 8,
		Flip = 9,
		Impulse = 10,
	};

	enum class MorphCategory : uint8_t
	{
		ReservedCategory = 0,
		Eyebrow = 1,
		Eye = 2,
		Mouth = 3,
		Other = 4,
	};

	class PmxMorphOffset
	{
	public:
		void virtual Read(std::istream* stream, PmxSetting* setting) = 0;
	};

	class PmxMorphVertexOffset : public PmxMorphOffset
	{
	public:
		PmxMorphVertexOffset()
			: vertex_index(0)
		{
			for (int i = 0; i < 3; ++i) {
				position_offset[i] = 0.0f;
			}
		}
		int vertex_index;
		float position_offset[3];
		void Read(std::istream* stream, PmxSetting* setting) override;
	};

	class PmxMorphUVOffset : public PmxMorphOffset
	{
	public:
		PmxMorphUVOffset()
			: vertex_index(0)
		{
			for (int i = 0; i < 4; ++i) {
				uv_offset[i] = 0.0f;
			}
		}
		int vertex_index;
		float uv_offset[4];
		void Read(std::istream* stream, PmxSetting* setting) override;
	};

	class PmxMorphBoneOffset : public PmxMorphOffset
	{
	public:
		PmxMorphBoneOffset()
			: bone_index(0)
		{
			for (int i = 0; i < 3; ++i) {
				translation[i] = 0.0f;
			}
			for (int i = 0; i < 4; ++i) {
				rotation[i] = 0.0f;
			}
		}
		int bone_index;
		float translation[3];
		float rotation[4];
		void Read(std::istream* stream, PmxSetting* setting) override;
	};

	class PmxMorphMaterialOffset : public PmxMorphOffset
	{
	public:
		PmxMorphMaterialOffset()
			: specularity(0.0f)
			, edge_size(0.0f)
		{
			for (int i = 0; i < 3; ++i) {
				specular[i] = 0.0f;
				ambient[i] = 0.0f;
			}
			for (int i = 0; i < 4; ++i) {
				diffuse[i] = 0.0f;
				edge_color[i] = 0.0f;
				texture_argb[i] = 0.0f;
				sphere_texture_argb[i] = 0.0f;
				toon_texture_argb[i] = 0.0f;
			}
		}
		int material_index;
		uint8_t offset_operation;
		float diffuse[4];
		float specular[3];
		float specularity;
		float ambient[3];
		float edge_color[4];
		float edge_size;
		float texture_argb[4];
		float sphere_texture_argb[4];
		float toon_texture_argb[4];
		void Read(std::istream* stream, PmxSetting* setting) override;
	};

	class PmxMorphGroupOffset : public PmxMorphOffset
	{
	public:
		PmxMorphGroupOffset()
			: morph_index(0)
			, morph_weight(0.0f)
		{}
		int morph_index;
		float morph_weight;
		void Read(std::istream* stream, PmxSetting* setting) override;
	};

	class PmxMorphFlipOffset : public PmxMorphOffset
	{
	public:
		PmxMorphFlipOffset()
			: morph_index(0)
			, morph_value(0.0f)
		{}
		int morph_index;
		float morph_value;
		void Read(std::istream* stream, PmxSetting* setting) override;
	};

	class PmxMorphImpulseOffset : public PmxMorphOffset
	{
	public:
		PmxMorphImpulseOffset()
			: rigid_body_index(0)
			, is_local(0)
		{
			for (int i = 0; i < 3; ++i) {
				velocity[i] = 0.0f;
				angular_torque[i] = 0.0f;
			}
		}
		int rigid_body_index;
		uint8_t is_local;
		float velocity[3];
		float angular_torque[3];
		void Read(std::istream* stream, PmxSetting* setting) override;
	};

	class PmxMorph
	{
	public:
		PmxMorph()
			: offset_count(0)
		{
		}
		utfstring morph_name;
		utfstring morph_english_name;
		MorphCategory category;
		MorphType morph_type;
		int offset_count;
		std::unique_ptr<PmxMorphVertexOffset[]> vertex_offsets;
		std::unique_ptr<PmxMorphUVOffset[]> uv_offsets;
		std::unique_ptr<PmxMorphBoneOffset[]> bone_offsets;
		std::unique_ptr<PmxMorphMaterialOffset[]> material_offsets;
		std::unique_ptr<PmxMorphGroupOffset[]> group_offsets;
		std::unique_ptr<PmxMorphFlipOffset[]> flip_offsets;
		std::unique_ptr<PmxMorphImpulseOffset[]> Impulse_offsets;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	class PmxFrameElement
	{
	public:
		PmxFrameElement()
			: element_target(0)
			, index(0)
		{
		}
		uint8_t element_target;
		int index;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	class PmxFrame
	{
	public:
		PmxFrame()
			: frame_flag(0)
			, element_count(0)
		{
		}
		utfstring frame_name;
		utfstring frame_english_name;
		uint8_t frame_flag;
		int element_count;
		std::unique_ptr<PmxFrameElement[]> elements;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	class PmxRigidBody
	{
	public:
		PmxRigidBody()
			: target_bone(0)
			, group(0)
			, mask(0)
			, shape(0)
			, mass(0.0f)
			, move_attenuation(0.0f)
			, rotation_attenuation(0.0f)
			, repulsion(0.0f)
			, friction(0.0f)
			, physics_calc_type(0)
		{
			for (int i = 0; i < 3; ++i) {
				size[i] = 0.0f;
				position[i] = 0.0f;
				orientation[i] = 0.0f;
			}
		}
		utfstring rigid_body_name;
		utfstring rigid_body_english_name;
		int target_bone;
		uint8_t group;
		uint16_t mask;
		uint8_t shape;
		float size[3];
		float position[3];
		float orientation[3];
		float mass;
		float move_attenuation;
		float rotation_attenuation;
		float repulsion;
		float friction;
		uint8_t physics_calc_type;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	enum class PmxJointType : uint8_t
	{
		Generic6DofSpring = 0,
		Generic6Dof = 1,
		Point2Point = 2,
		ConeTwist = 3,
		Slider = 5,
		Hinge = 6
	};

	class PmxJointParam
	{
	public:
		PmxJointParam()
			: rigid_body1(0)
			, rigid_body2(0)
		{
			for (int i = 0; i < 3; ++i) {
				position[i] = 0.0f;
				orientaiton[i] = 0.0f;
				move_limitation_min[i] = 0.0f;
				move_limitation_max[i] = 0.0f;
				rotation_limitation_min[i] = 0.0f;
				rotation_limitation_max[i] = 0.0f;
				spring_move_coefficient[i] = 0.0f;
				spring_rotation_coefficient[i] = 0.0f;
			}
		}
		int rigid_body1;
		int rigid_body2;
		float position[3];
		float orientaiton[3];
		float move_limitation_min[3];
		float move_limitation_max[3];
		float rotation_limitation_min[3];
		float rotation_limitation_max[3];
		float spring_move_coefficient[3];
		float spring_rotation_coefficient[3];
		void Read(std::istream* stream, PmxSetting* setting);
	};

	class PmxJoint
	{
	public:
		utfstring joint_name;
		utfstring joint_english_name;
		PmxJointType joint_type;
		PmxJointParam param;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	enum PmxSoftBodyFlag : uint8_t
	{
		BLink = 0x01,
		Cluster = 0x02,
		Link = 0x04
	};

	class PmxAnchorRigidBody
	{
	public:
		PmxAnchorRigidBody()
			: related_rigid_body(0)
			, related_vertex(0)
			, is_near(false)
		{}
		int related_rigid_body;
		int related_vertex;
		bool is_near;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	class PmxSoftBody
	{
	public:
		PmxSoftBody()
			: shape(0)
			, target_material(0)
			, group(0)
			, mask(0)
			, blink_distance(0)
			, cluster_count(0)
			, mass(0.0)
			, collisioni_margin(0.0)
			, aero_model(0)
			, VCF(0.0f)
			, DP(0.0f)
			, DG(0.0f)
			, LF(0.0f)
			, PR(0.0f)
			, VC(0.0f)
			, DF(0.0f)
			, MT(0.0f)
			, CHR(0.0f)
			, KHR(0.0f)
			, SHR(0.0f)
			, AHR(0.0f)
			, SRHR_CL(0.0f)
			, SKHR_CL(0.0f)
			, SSHR_CL(0.0f)
			, SR_SPLT_CL(0.0f)
			, SK_SPLT_CL(0.0f)
			, SS_SPLT_CL(0.0f)
			, V_IT(0)
			, P_IT(0)
			, D_IT(0)
			, C_IT(0)
			, LST(0.0f)
			, AST(0.0f)
			, VST(0.0f)
			, anchor_count(0)
			, pin_vertex_count(0)
		{}
		utfstring soft_body_name;
		utfstring soft_body_english_name;
		uint8_t shape;
		int target_material;
		uint8_t group;
		uint16_t mask;
		PmxSoftBodyFlag flag;
		int blink_distance;
		int cluster_count;
		float mass;
		float collisioni_margin;
		int aero_model;
		float VCF;
		float DP;
		float DG;
		float LF;
		float PR;
		float VC;
		float DF;
		float MT;
		float CHR;
		float KHR;
		float SHR;
		float AHR;
		float SRHR_CL;
		float SKHR_CL;
		float SSHR_CL;
		float SR_SPLT_CL;
		float SK_SPLT_CL;
		float SS_SPLT_CL;
		int V_IT;
		int P_IT;
		int D_IT;
		int C_IT;
		float LST;
		float AST;
		float VST;
		int anchor_count;
		std::unique_ptr<PmxAnchorRigidBody[]> anchors;
		int pin_vertex_count;
		std::unique_ptr<int[]> pin_vertices;
		void Read(std::istream* stream, PmxSetting* setting);
	};

	class PmxLoader
	{
	public:
		PmxLoader()
			: version(0.0f)
			, vertex_count(0)
			, index_count(0)
			, texture_count(0)
			, material_count(0)
			, bone_count(0)
			, morph_count(0)
			, frame_count(0)
			, rigid_body_count(0)
			, joint_count(0)
			, soft_body_count(0)
		{}

		float version;
		PmxSetting setting;
		utfstring model_name;
		utfstring model_english_name;
		utfstring model_comment;
		utfstring model_english_comment;
		int vertex_count;
		std::unique_ptr<PmxVertex[]> vertices;
		int index_count;
		std::unique_ptr<int[]> indices;
		int texture_count;
		std::unique_ptr< utfstring[]> textures;
		int material_count;
		std::unique_ptr<PmxMaterial[]> materials;
		int bone_count;
		std::unique_ptr<PmxBone[]> bones;
		int morph_count;
		std::unique_ptr<PmxMorph[]> morphs;
		int frame_count;
		std::unique_ptr<PmxFrame[] > frames;
		int rigid_body_count;
		std::unique_ptr<PmxRigidBody[]> rigid_bodies;
		int joint_count;
		std::unique_ptr<PmxJoint[]> joints;
		int soft_body_count;
		std::unique_ptr<PmxSoftBody[]> soft_bodies;
		void Init();
		void Read(std::istream* stream);
	};
}

namespace fbx
{
	class FBXLoader
	{
	private:
#pragma region Part_Of_Texture

		// Surface에서 Material 추출. (GetSrcObject<FbxSurfaceMaterial>)
		FbxSurfaceMaterial* smat;
		// Mesh 속성 
		FbxProperty prop;
		FbxLayeredTexture* layered_texture;
		FbxFileTexture* fTexture;

		int layered_texture_count;
		int texture_count;

#pragma endregion

#pragma region Part_Of_Animation

		// For all skins and all clusters, accumulate their deformation and weight
		// on each vertices and store them in lClusterDeformation and lClusterWeight.
		FbxCluster* lCluster;
		int lClusterIDX;
		int FrameIDX;

		fbxsdk::FbxAMatrix lVertexTransformMatrix;
		fbxsdk::FbxAMatrix lClusterRelativeInitPosition;

		int lIndex;
		double lWeight;
		fbxsdk::FbxAMatrix lInfluence;

		fbxsdk::FbxAMatrix lReferenceGlobalInitPosition;
		fbxsdk::FbxAMatrix lReferenceGlobalCurrentPosition;
		fbxsdk::FbxAMatrix lAssociateGlobalInitPosition;
		fbxsdk::FbxAMatrix lAssociateGlobalCurrentPosition;
		fbxsdk::FbxAMatrix lClusterGlobalInitPosition;
		fbxsdk::FbxAMatrix lClusterGlobalCurrentPosition;

		fbxsdk::FbxAMatrix lReferenceGeometry;
		fbxsdk::FbxAMatrix lAssociateGeometry;
		fbxsdk::FbxAMatrix lClusterGeometry;

		fbxsdk::FbxAMatrix lClusterRelativeCurrentPositionInverse;

		// All the links must have the same link mode.
		FbxCluster::ELinkMode lClusterMode;
		int lVertexCount;
		std::vector<double>				lWeightAccumulation;
		bool passCondition;

		int threadIDX;

#pragma endregion

		///////////////////////////////////////////////////////////////////////////////

	public:
		FBXLoader() = default;
		~FBXLoader() = default;

		ObjectData* CreateFBXModel(
			_Out_ ObjectData* newData,
			_In_ std::string Name,
			_In_  std::string Path
		);
		// LoadFBXSkinnedModel은 일시적으로 오브젝트 데이터를 제공하기만 하고
		// Memory Pool에는 따로 업로드 하지 않습니다.
		// 사용 후엔 꼭 Release 하십시오.
		ObjectData* LoadFBXModel(
			ObjectData* newData,
			std::string Name,
			std::string Path
		);

		ObjectData* CreateFBXSkinnedModel(
			_Out_ ObjectData* newData,
			_In_ std::string Name,
			_In_  std::string Path
		);
		// LoadFBXSkinnedModel은 일시적으로 오브젝트 데이터를 제공하기만 하고
		// Memory Pool에는 따로 업로드 하지 않습니다.
		// 사용 후엔 꼭 Release 하십시오.
		ObjectData* LoadFBXSkinnedModel(
			std::string Name,
			std::string Path
		);

		int ExtractedAnimationBone(
			_In_ std::string Path,
			_In_ std::string targetPath,
			_Out_ FbxArray<FbxString*>& animNameLists,
			_Out_ std::vector<FbxTime>& mStarts,
			_Out_ std::vector<FbxTime>& mStops,
			_Out_ std::vector<long long>& countOfFrame
		);

	private:
		FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex);

		FbxAMatrix GetGlobalPosition(
			FbxNode* pNode,
			const FbxTime& pTime,
			FbxPose* pPose = 0,
			FbxAMatrix* pParentGlobalPosition = 0
		);

		void DrawNodeRecursive(
			FbxNode* pNode
		);

		void DrawNodeRecursive(
			FbxNode* pNode,
			FbxTime& pTime,
			FbxPose* pPose,
			Tree<Skeleton>* pBoneHierarchy
		);

		void DrawBoneRecursive(
			FbxNode* pNode,
			std::vector<FbxTime> mStops,
			FbxTime& pTime,
			FbxAMatrix& pGlobalRootPositions,
			std::vector<FbxAMatrix>& pParentGlobalPositions,
			FbxPose* pPose,
			long long perFrame,
			std::ofstream& outFile
		);

		int DrawMesh(
			FbxNode* pNode,
			MeshData& meshData,
			bool uvMode
		);

		void DrawMesh(
			int submeshIDX,
			FbxNode* pNode,
			ObjectData* meshAnimData
		);

		void ComputeShapeDeformation(
			FbxMesh* pMesh,
			FbxTime& pTime,
			FbxAnimLayer* pAnimLayer,
			FbxVector4* pVertexArray);
		void ComputeClusterDeformation(
			FbxAMatrix& pGlobalPosition,
			FbxMesh* pMesh,
			FbxCluster* pCluster,
			FbxAMatrix& pVertexTransformMatrix,
			FbxTime pTime);
		void ComputeLinearDeformation(
			FbxAMatrix& pGlobalPosition,
			FbxMesh* pMesh,
			FbxTime& pTime,
			FbxVector4* pVertexArray);

		// Draw Skeleton
		void DrawSkeleton(
			FbxNode* pNode,
			FbxSkeleton* skeleton,
			int targetBoneCount,
			pmx::PmxBone* targetBone,
			std::vector<FbxTime> mStarts,
			std::vector<FbxTime> mStops,
			FbxTime& pTime,
			FbxAnimLayer* pAnimLayer,
			FbxAMatrix& pParentGlobalPosition,
			FbxAMatrix& pGlobalPosition,
			long long perFrame,
			std::ofstream& outFile
		);

		void DrawCamera(
			FbxNode* pNode,
			FbxTime& pTime,
			FbxAnimLayer* pAnimLayer,
			FbxAMatrix& pGlobalPosition
		);

		void DrawBone(
			FbxNode* pNode,
			std::vector<FbxTime> mStops,
			FbxAMatrix& pOriginGlobalPosition,
			FbxAMatrix& pParentGlobalPosition,
			long long perFrame,
			std::ofstream& outFile
		);

	private:
		inline void MatrixScale(FbxAMatrix& pMatrix, double pValue)
		{
			int i, j;

			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					pMatrix[i][j] *= pValue;
				}
			}
		}

		inline void MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue)
		{
			pMatrix[0][0] += pValue;
			pMatrix[1][1] += pValue;
			pMatrix[2][2] += pValue;
			pMatrix[3][3] += pValue;
		}

		inline void MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix)
		{
			int i, j;

			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					pDstMatrix[i][j] += pSrcMatrix[i][j];
				}
			}
		}
	};
}
#pragma once

#include <windows.h>

#include <stdint.h>

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "Physx.h"
#include "Tree.h"

#define IMGUI_API

//#define _WIN32_IE 0x0300

#define _HWND_BUTTON_OFFSET		1000
#define _HWND_EDIT_OFFSET		1300
#define _HWND_LIST_BOX_OFFSET	1500
#define _HWND_TRACKBAR_OFFSET	1700

#define _TAB_COUNT 5

/**
 *
 * @brief KeyBoard Input Action, Axis를 맵핑한다.
 * @details 키보드의 특정 키에 대하여 이름을 부여한다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
enum GameAction {
	GameActionMoveCamFwd,
	GameActionMoveCamBack,
	GameActionMoveCamLeft,
	GameActionMoveCamRight,
	GameActionTurnCamLeft,
	GameActionTurnCamRight,
	GameActionLookUp,
	GameActionLookDown,
	GameActionRaiseCam,
	GameActionLowerCam,
	GameActionCount
};

/**
 *
 * @brief Mouse Input Action, Axis를 맵핑한다.
 * @details 마우스의 특정 키에 대하여 이름을 부여한다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
enum MouseAction {
	MouseActionLeftDown,
	MouseActionLeftUp,
	MouseActionMiddleDown,
	MouseActionMiddleUp,
	MouseActionRightDown,
	MouseActionRightUp,
	MouseActionWheelUp,
	MouseActionWheelDown,
	MouseActionCount
};

using uint16 = std::uint16_t;
using uint32 = std::uint32_t;

/**
 *
 * @brief Vertex에 대한 정보가 담겨있다.
 * @details 위치, 노말, TexC등 Vertex에 필요한 정보가 담겨있다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
#pragma pack(push, 1)
struct Vertex
{
	float Position[3];
	float Normal[3];
	float TangentU[3];
	float TexCoord0[2];
	float TexCoord1[2];
	float TexCoord2[2];
	float TexCoord3[2];

	// Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)

	Vertex()
	{
		Position[0] = 0.0f;
		Position[1] = 0.0f;
		Position[2] = 0.0f;

		Normal[0] = 0.0f;
		Normal[1] = 0.0f;
		Normal[2] = 0.0f;

		TangentU[0] = 0.0f;
		TangentU[1] = 0.0f;
		TangentU[2] = 0.0f;

		TexCoord0[0] = 0.0f;
		TexCoord0[1] = 0.0f;

		TexCoord1[0] = 0.0f;
		TexCoord1[1] = 0.0f;

		TexCoord2[0] = 0.0f;
		TexCoord2[1] = 0.0f;

		TexCoord3[0] = 0.0f;
		TexCoord3[1] = 0.0f;
	}

	Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float tz, float u, float v)
	{
		Position[0] = px;
		Position[1] = py;
		Position[2] = pz;

		Normal[0] = nx;
		Normal[1] = ny;
		Normal[2] = nz;

		TangentU[0] = tx;
		TangentU[1] = ty;
		TangentU[2] = tz;

		TexCoord0[0] = u;
		TexCoord0[1] = v;

		TexCoord1[0] = 0.0f;
		TexCoord1[1] = 0.0f;

		TexCoord2[0] = 0.0f;
		TexCoord2[1] = 0.0f;

		TexCoord3[0] = 0.0f;
		TexCoord3[1] = 0.0f;
	}
};
#pragma pack(pop)

/**
 *
 * @brief Instance에 대한 정보가 담겨있다.
 * @details 로컬에 대한 위치, 회전, 스케일 정보가 담겨있다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
#pragma pack(push, 1)
struct Instance
{
	float Position[3];
	float Rotation[3];
	float Scale[3];
};
#pragma pack(pop)

/**
 *
 * @brief Material에 대한 정보가 담겨있다.
 * @details 메쉬의 재질에 대한 정보가 담겨있다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
struct Material
{
	// 텍스쳐의 종류는 Base, Normal, Masked, Metallic, Roughness 등이 있다.
	// 하나의 서브메쉬에 두 개 이상의 텍스쳐가 들어갈 수 있다.
	// 최대 텍스쳐 적재 개수 (슬롯 개수)는 128개

	std::vector<std::string> BaseColorTexutures;
	std::vector<std::string> NormalTexutures;
	std::vector<std::string> MaskedTextures;
	std::vector<std::string> MetallicTextures;
	std::vector<std::string> RoughnessTextures;

};

/**
 *
 * @brief 하나의 메쉬 덩어리에 대한 정보
 * @details 버텍스와 인덱스의 집합으로 하나의 메쉬 덩어리를 가지고 있다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
struct MeshData
{
	std::string mName;
	Material mMaterial;

	std::vector<Vertex> Vertices;
	std::vector<uint32> Indices32;
};

/**
 *
 * @brief Shader에 대한 정보가 담겨있다.
 * @details 메쉬의 셰이더 코드에 대한 정보가 담겨있다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
struct Shader
{
	ID3DBlob* ShaderCode;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
};

/**
 *
 * @brief 직사광선에 대한 정보가 담겨있다..
 * @details 빛과 관련된 정보가 담겨있다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
struct DirectionalLight
{
	float4		color;

	float dummy;
	float ambientStrength;
	float specularStrength;
	float specularExponent;

	float4		dirEye;
	float4x4	lightViewMatrix;
	float4x4	lightProjectionMatrix;
};

/**
 *
 * @brief 포인트라이트에 대한 정보가 담겨있다..
 * @details 빛과 관련된 정보가 담겨있다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
struct PointLight
{
	float4		color;

	float dummy;
	float ambientStrength;
	float specularStrength;
	float specularExponent;

	float4		posEye;
	float4x4	lightViewMatrix;
	float4x4	lightProjectionMatrix;
};

/**
 *
 * @brief 모든 라이트에 대한 정보가
 * @details 빛과 관련된 정보가 담겨있다.
 * @date 2023-06-08
 * @version 0.0.1
 *
 */
#pragma pack(push, 1)
struct LightData
{
	int dummy[2];
	int dirLightNum;
	int pointLightNum;
	DirectionalLight dirLight[150];
	PointLight pointLights[150];
};
#pragma pack(pop)

enum ObjectType {
	OBJ_TYPE,
	FBX_TYPE,
	FBX_SKINNED_TYPE,
	PMX_TYPE
};

enum PhysicsType {
	NONE_TYPE,
	STATIC_TYPE,
	KINEMATIC_TYPE,
	DYNAMIC_TYPE,
	PARTICLE_TYPE
};

enum UIType {
	UI_DEFAULT = 0,
	UI_BUTTON,
	UI_EDIT,
	UI_LISTBOX,
	UI_TRACKBAR,
};

static bool global_windowDidResize = false;

class UI_HWND
{
public:
	UIType type;
	HWND hwnd;

	std::string name;

public:
	UI_HWND() {
		this->name = "";
	}

	virtual unsigned int getHWND_ID() {
		return 0;
	}
	void setVisible(bool isVisible);
};

struct Skeleton
{
public:
	// TranslationMatrix는 애니메이션 원본 매트릭스
	float4x4 TranslationMatrix;

	// 메쉬에 투영된 뼈대의 위치
	float4 Position;
	float4 Rotation;
	float4 Scale;

	////////////////////////////////////////////////////////////////////
	// 
	//	DeltaTranslationMatrix는 원본 매트릭스에 대한 델타 매트릭스
	//	이후 UpdateAnimation에서 해당 뼈대는 다음과 같이 계산 된다.
	// 
	//	B3 Skeleton의 Delta가 Update 되었을 때
	// 
	//	B1 = B1
	//	B2 = B2
	//	B3 = B3 * DeltaTranslationMatrix
	//	B4 = B3 * DeltaTranslationMatrix * ((B3)^(-1) * B4)
	//	B5 = B3 * DeltaTranslationMatrix * ((B3)^(-1) * B5)
	// 
	////////////////////////////////////////////////////////////////////

	float3 DeltaPosition = { 0.0f, 0.0f, 0.0f };
	float3 DeltaRotation = { 0.0f, 0.0f, 0.0f };
	float3 DeltaScale = { 1.0f, 1.0f, 1.0f };

	float4x4 DeltaTranslationMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	float4x4 DeltaWeightMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

public:
	std::string					Name;
	UINT						Level;
	Tree<Skeleton>* Parent;
	std::vector<Skeleton*>		Childs;

public:
	Skeleton() {
		Level = 0;
		Parent = nullptr;
		Position = {0.0f, 0.0f, 0.0f, 0.0f};

		DeltaPosition = { 0.0f, 0.0f, 0.0f };
		DeltaRotation = { 0.0f, 0.0f, 0.0f };
		DeltaScale = { 1.0f, 1.0f, 1.0f };

		DeltaTranslationMatrix = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		DeltaWeightMatrix = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

public:
	void SetSkeletalTranslation(float3 position) {
		DeltaPosition.x = position.x;
		DeltaPosition.y = position.y;
		DeltaPosition.z = position.z;

		DeltaTranslationMatrix = makeAffineMatrix(
			DeltaPosition, DeltaRotation, DeltaScale
		);
	}
	void SetSkeletalRotation(float3 rotation) {
		DeltaRotation.x = rotation.x;
		DeltaRotation.y = rotation.y;
		DeltaRotation.z = rotation.z;

		DeltaTranslationMatrix = makeAffineMatrix(
			DeltaPosition, DeltaRotation, DeltaScale
		);
	}
	void SetSkeletalScale(float3 scale) {
		DeltaScale.x = scale.x;
		DeltaScale.y = scale.y;
		DeltaScale.z = scale.z;

		DeltaTranslationMatrix = makeAffineMatrix(
			DeltaPosition, DeltaRotation, DeltaScale
		);
	}

	void AddSkeletalTranslation(float3 position) {
		DeltaPosition.x = DeltaPosition.x + position.x;
		DeltaPosition.y = DeltaPosition.y + position.y;
		DeltaPosition.z = DeltaPosition.z + position.z;

		DeltaTranslationMatrix = makeAffineMatrix(
			DeltaPosition, DeltaRotation, DeltaScale
		);
	}
	void AddSkeletalRotation(float3 rotation) {
		DeltaPosition.x = (DeltaPosition.x + rotation.x) < 360.0f ? (DeltaPosition.x + rotation.x) : 0.0f;
		DeltaPosition.y = (DeltaPosition.y + rotation.y) < 360.0f ? (DeltaPosition.y + rotation.y) : 0.0f;
		DeltaPosition.z = (DeltaPosition.z + rotation.z) < 360.0f ? (DeltaPosition.z + rotation.z) : 0.0f;

		DeltaTranslationMatrix = makeAffineMatrix(
			DeltaPosition, DeltaRotation, DeltaScale
		);
	}
	void AddSkeletalScale(float3 scale) {
		DeltaPosition.x = DeltaPosition.x + scale.x;
		DeltaPosition.y = DeltaPosition.y + scale.y;
		DeltaPosition.z = DeltaPosition.z + scale.z;

		DeltaTranslationMatrix = makeAffineMatrix(
			DeltaPosition, DeltaRotation, DeltaScale
		);
	}
};

/*
	Object의 인스턴스 데이터를 담는 테이블 클래스

	상속, 다형성 X 
	(ObjectData에서 다른 클래스로 업 캐스팅 될 경우, Transform도 따로 캐스팅이 되어야 함. 
		번거로워지고 느려짐. 코드 길이도 길어짐)
	(왠만해선 Transform은 다형성을 고려하지 않는 걸로...) 

	Physx와 연동시키기 위해서는 
*/

class Transform
{
public:
	class transform
	{
	public:
		// 월드 좌표계
		/*
			월드 좌표계가 움직이는 경우
			1. 지면이 움직이는 경우
			2. 바인드 된, Parent Socket이 움직이는 경우
			3. Vehicle에 탔을 경우
		*/
		Instance* GlobalMatrix;
		// 국소 좌표계
		/*
			일반적인 인스턴스의 움직임은 국소 좌표계에서 처리
		*/
		Instance* LocalMatrix;
		// 월드 + 국소 좌표계
		/*
			최종 결과 Transform
		*/
		Instance* Matrix;

		float3* Velocity;
		float3* Torque;
		float3* Accumulate;

		bool bIsDirtyInstance = false;

	public:
		transform() {

			GlobalMatrix				= new Instance();
			LocalMatrix					= new Instance();
			Matrix						= new Instance();

			Velocity					= new float3();
			Torque						= new float3();
			Accumulate					= new float3();

			GlobalMatrix->Position[0]	= 0.0f;
			GlobalMatrix->Position[1]	= 0.0f;
			GlobalMatrix->Position[2]	= 0.0f;

			GlobalMatrix->Rotation[0]	= 0.0f;
			GlobalMatrix->Rotation[1]	= 0.0f;
			GlobalMatrix->Rotation[2]	= 0.0f;

			GlobalMatrix->Scale[0]		= 1.0f;
			GlobalMatrix->Scale[1]		= 1.0f;
			GlobalMatrix->Scale[2]		= 1.0f;

			LocalMatrix->Position[0]	= 0.0f;
			LocalMatrix->Position[1]	= 0.0f;
			LocalMatrix->Position[2]	= 0.0f;

			LocalMatrix->Rotation[0]	= 0.0f;
			LocalMatrix->Rotation[1]	= 0.0f;
			LocalMatrix->Rotation[2]	= 0.0f;

			LocalMatrix->Scale[0]		= 1.0f;
			LocalMatrix->Scale[1]		= 1.0f;
			LocalMatrix->Scale[2]		= 1.0f;

			*Velocity					= { 0.0f, 0.0f, 0.0f };
			*Torque						= { 0.0f, 0.0f, 0.0f };
			*Accumulate					= { 0.0f, 0.0f, 0.0f };
		}

		~transform() {

			delete GlobalMatrix;
			delete LocalMatrix;

			delete Velocity;
			delete Torque;
			delete Accumulate;

		}

	public:

		inline void GetPosition(float& x, float& y, float& z)
		{
			x = GlobalMatrix->Position[0] + LocalMatrix->Position[0];
			y = GlobalMatrix->Position[1] + LocalMatrix->Position[1];
			z = GlobalMatrix->Position[2] + LocalMatrix->Position[2];
		}
		inline void GetRotation(float& x, float& y, float& z)
		{
			x = GlobalMatrix->Rotation[0] + LocalMatrix->Rotation[0];
			y = GlobalMatrix->Rotation[1] + LocalMatrix->Rotation[1];
			z = GlobalMatrix->Rotation[2] + LocalMatrix->Rotation[2];

			ClampVector3(x, y, z, 0.0f, 360.0f);
		}
		inline void GetQuaternion(float& x, float& y, float& z, float& w)
		{
			float3 rotation;
			rotation.x = GlobalMatrix->Rotation[0] + LocalMatrix->Rotation[0];
			rotation.y = GlobalMatrix->Rotation[1] + LocalMatrix->Rotation[1];
			rotation.z = GlobalMatrix->Rotation[2] + LocalMatrix->Rotation[2];

			ClampVector3(rotation, 0.0f, 360.0f);

			float4 quaternion = Quaternion(
				rotation.x,
				rotation.y,
				rotation.z
			);

			x = quaternion.x;
			y = quaternion.y;
			z = quaternion.z;
			w = quaternion.w;

		}
		inline void GetScale(float& x, float& y, float& z)
		{
			x = GlobalMatrix->Scale[0] * LocalMatrix->Scale[0];
			y = GlobalMatrix->Scale[1] * LocalMatrix->Scale[1];
			z = GlobalMatrix->Scale[2] * LocalMatrix->Scale[2];
		}

		inline void GetGlobalPosition(float& x, float& y, float& z)
		{
			x = GlobalMatrix->Position[0];
			y = GlobalMatrix->Position[1];
			z = GlobalMatrix->Position[2];
		}
		inline void GetGlobalRotation(float& x, float& y, float& z)
		{
			x = GlobalMatrix->Rotation[0];
			y = GlobalMatrix->Rotation[1];
			z = GlobalMatrix->Rotation[2];
		}
		inline void GetGlobalQuaternion(float& x, float& y, float& z, float& w)
		{
			float4 quaternion = Quaternion(
				GlobalMatrix->Rotation[0],
				GlobalMatrix->Rotation[1],
				GlobalMatrix->Rotation[2]
			);

			x = quaternion.x;
			y = quaternion.y;
			z = quaternion.z;
			w = quaternion.w;
		}
		inline void GetGlobalScale(float& x, float& y, float& z)
		{
			x = GlobalMatrix->Scale[0];
			y = GlobalMatrix->Scale[1];
			z = GlobalMatrix->Scale[2];
		}

		inline void GetLocalPosition(float& x, float& y, float& z)
		{
			x = LocalMatrix->Position[0];
			y = LocalMatrix->Position[1];
			z = LocalMatrix->Position[2];
		}
		inline void GetLocalRotation(float& x, float& y, float& z)
		{
			x = LocalMatrix->Rotation[0];
			y = LocalMatrix->Rotation[1];
			z = LocalMatrix->Rotation[2];
		}
		inline void GetLocalQuaternion(float& x, float& y, float& z, float& w)
		{
			float4 quaternion = Quaternion(
				LocalMatrix->Rotation[0],
				LocalMatrix->Rotation[1],
				LocalMatrix->Rotation[2]
			);

			x = quaternion.x;
			y = quaternion.y;
			z = quaternion.z;
			w = quaternion.w;
		}
		inline void GetLocalScale(float& x, float& y, float& z)
		{
			x = LocalMatrix->Scale[0];
			y = LocalMatrix->Scale[1];
			z = LocalMatrix->Scale[2];
		}

		// Linear Velocity
		inline void GetVelocity(float& x, float& y, float& z)
		{
			x = Velocity->x;
			y = Velocity->y;
			z = Velocity->z;
		}
		// Angular Velocity
		inline void GetTorque(float& x, float& y, float& z)
		{
			x = Torque->x;
			y = Torque->y;
			z = Torque->z;
		}
		inline void GetAccumulate(float& x, float& y, float& z)
		{
			x = Accumulate->x;
			y = Accumulate->y;
			z = Accumulate->z;
		}

		inline void GetPosition(float3& position)
		{
			position.x = GlobalMatrix->Position[0] + LocalMatrix->Position[0];
			position.y = GlobalMatrix->Position[1] + LocalMatrix->Position[1];
			position.z = GlobalMatrix->Position[2] + LocalMatrix->Position[2];
		}
		inline void GetRotation(float3& rotation)
		{
			rotation.x = GlobalMatrix->Rotation[0] + LocalMatrix->Rotation[0];
			rotation.y = GlobalMatrix->Rotation[1] + LocalMatrix->Rotation[1];
			rotation.z = GlobalMatrix->Rotation[2] + LocalMatrix->Rotation[2];

			ClampVector3(rotation, 0.0f, 360.0f);
		}
		inline void GetQuaternion(float4& quaternion)
		{
			float3 rotation;
			rotation.x = GlobalMatrix->Rotation[0] + LocalMatrix->Rotation[0];
			rotation.y = GlobalMatrix->Rotation[1] + LocalMatrix->Rotation[1];
			rotation.z = GlobalMatrix->Rotation[2] + LocalMatrix->Rotation[2];

			ClampVector3(rotation, 0.0f, 360.0f);

			quaternion = Quaternion(
				rotation.x,
				rotation.y,
				rotation.z
			);
		}
		inline void GetScale(float3& scale)
		{
			scale.x = GlobalMatrix->Scale[0] * LocalMatrix->Scale[0];
			scale.y = GlobalMatrix->Scale[1] * LocalMatrix->Scale[1];
			scale.z = GlobalMatrix->Scale[2] * LocalMatrix->Scale[2];
		}

		inline void GetGlobalPosition(float3& position)
		{
			position.x = GlobalMatrix->Position[0];
			position.y = GlobalMatrix->Position[1];
			position.z = GlobalMatrix->Position[2];
		}
		inline void GetGlobalRotation(float3& rotation)
		{
			rotation.x = GlobalMatrix->Rotation[0];
			rotation.y = GlobalMatrix->Rotation[1];
			rotation.z = GlobalMatrix->Rotation[2];
		}
		inline void GetGlobalQuaternion(float4& quaternion)
		{
			quaternion = Quaternion(
				GlobalMatrix->Rotation[0],
				GlobalMatrix->Rotation[1],
				GlobalMatrix->Rotation[2]
			);
		}
		inline void GetGlobalScale(float3& scale)
		{
			scale.x = GlobalMatrix->Scale[0];
			scale.y = GlobalMatrix->Scale[1];
			scale.z = GlobalMatrix->Scale[2];
		}

		inline void GetLocalPosition(float3& position)
		{
			position.x = LocalMatrix->Position[0];
			position.y = LocalMatrix->Position[1];
			position.z = LocalMatrix->Position[2];
		}
		inline void GetLocalRotation(float3& rotation)
		{
			rotation.x = LocalMatrix->Rotation[0];
			rotation.y = LocalMatrix->Rotation[1];
			rotation.z = LocalMatrix->Rotation[2];
		}
		inline void GetLocalQuaternion(float4& quaternion)
		{
			quaternion = Quaternion(
				LocalMatrix->Rotation[0],
				LocalMatrix->Rotation[1],
				LocalMatrix->Rotation[2]
			);
		}
		inline void GetLocalScale(float3& scale)
		{
			scale.x = LocalMatrix->Scale[0];
			scale.y = LocalMatrix->Scale[1];
			scale.z = LocalMatrix->Scale[2];
		}

		// Linear Velocity
		inline void GetVelocity(float3& velocity) 
		{
			velocity.x = Velocity->x;
			velocity.y = Velocity->y;
			velocity.z = Velocity->z;
		}
		// Angular Velocity
		inline void GetTorque(float3& torque)
		{
			torque.x = Torque->x;
			torque.y = Torque->y;
			torque.z = Torque->z;
		}
		inline void GetAccumulate(float3& accumulate)
		{
			accumulate.x = Accumulate->x;
			accumulate.y = Accumulate->y;
			accumulate.z = Accumulate->z;
		}

		// Yaw Pitch Roll (ZXY)
		inline void GetLocalAffineMatrix(float4x4& affine)
		{
			float Pitch, Roll, Yaw;
			Pitch	= degreesToRadians(LocalMatrix->Rotation[0]);
			Roll	= degreesToRadians(LocalMatrix->Rotation[1]);
			Yaw		= degreesToRadians(LocalMatrix->Rotation[2]);

			affine = float4x4(
					LocalMatrix->Scale[0], 0.0f, 0.0f, 0.0f,
					0.0f, LocalMatrix->Scale[1], 0.0f, 0.0f,
					0.0f, 0.0f, LocalMatrix->Scale[2], 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				) *
				float4x4(
					cosf(Yaw), sinf(Yaw), 0.0f, 0.0f,
					-sinf(Yaw), cosf(Yaw), 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				) *
				float4x4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, cosf(Pitch), sinf(Pitch), 0.0f,
					0.0f, -sinf(Pitch), cosf(Pitch), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				) *
				float4x4(
					cosf(Roll), 0.0f, -sinf(Roll), 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					sinf(Roll), 0.0f, cosf(Roll), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				) *
				float4x4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					LocalMatrix->Position[0], LocalMatrix->Position[1], LocalMatrix->Position[2], 1.0f
				);
		}
		inline void GetGlobalAffineMatrix(float4x4& affine)
		{
			float Pitch, Roll, Yaw;
			Pitch = degreesToRadians(GlobalMatrix->Rotation[0]);
			Roll = degreesToRadians(GlobalMatrix->Rotation[1]);
			Yaw = degreesToRadians(GlobalMatrix->Rotation[2]);

			affine = float4x4(
					GlobalMatrix->Scale[0], 0.0f, 0.0f, 0.0f,
					0.0f, GlobalMatrix->Scale[1], 0.0f, 0.0f,
					0.0f, 0.0f, GlobalMatrix->Scale[2], 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				) *
				float4x4(
					cosf(Yaw), sinf(Yaw), 0.0f, 0.0f,
					-sinf(Yaw), cosf(Yaw), 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				) *
				float4x4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, cosf(Pitch), sinf(Pitch), 0.0f,
					0.0f, -sinf(Pitch), cosf(Pitch), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				) *
				float4x4(
					cosf(Roll), 0.0f, -sinf(Roll), 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					sinf(Roll), 0.0f, cosf(Roll), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				) *
				float4x4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					GlobalMatrix->Position[0], GlobalMatrix->Position[1], GlobalMatrix->Position[2], 1.0f
				);
		}
		inline void GetAffineMatrix(float4x4& affine)
		{
			float4x4 GlobalAffine, LocalAffine;

			GetGlobalAffineMatrix(GlobalAffine); 
			GetLocalAffineMatrix(LocalAffine);

			//affine = GlobalAffine * LocalAffine;

			affine = GlobalAffine;
		}

		inline void GetGlobalPosition(physx::PxVec3& position)
		{
			position.x = GlobalMatrix->Position[0];
			position.y = GlobalMatrix->Position[1];
			position.z = GlobalMatrix->Position[2];
		}
		inline void GetGlobalRotation(physx::PxVec3& rotation)
		{
			rotation.x = GlobalMatrix->Rotation[0];
			rotation.y = GlobalMatrix->Rotation[1];
			rotation.z = GlobalMatrix->Rotation[2];
		}
		inline void GetGlobalQuaternion(physx::PxQuat& quaternion)
		{
			float4 buffer;

			buffer = Quaternion(
				GlobalMatrix->Rotation[0],
				GlobalMatrix->Rotation[1],
				GlobalMatrix->Rotation[2]
			);

			quaternion.x = buffer.x;
			quaternion.y = buffer.y;
			quaternion.z = buffer.z;
			quaternion.w = buffer.w;
		}
		inline void GetGlobalScale(physx::PxVec3& scale)
		{
			scale.x = GlobalMatrix->Scale[0];
			scale.y = GlobalMatrix->Scale[1];
			scale.z = GlobalMatrix->Scale[2];
		}
		inline void GetGlobalTransform(physx::PxTransform& transform)
		{
			float4 buffer;

			buffer = Quaternion(
				GlobalMatrix->Rotation[0],
				GlobalMatrix->Rotation[1],
				GlobalMatrix->Rotation[2]
			);

			transform.p.x = GlobalMatrix->Position[0];
			transform.p.y = GlobalMatrix->Position[1];
			transform.p.z = GlobalMatrix->Position[2];

			transform.q.x = buffer.x;
			transform.q.y = buffer.y;
			transform.q.z = buffer.z;
			transform.q.w = buffer.w;
		}

		inline void GetLocalPosition(physx::PxVec3& position) 
		{
			position.x = LocalMatrix->Position[0];
			position.y = LocalMatrix->Position[1];
			position.z = LocalMatrix->Position[2];
		}
		inline void GetLocalRotation(physx::PxVec3& rotation)
		{
			rotation.x = LocalMatrix->Rotation[0];
			rotation.y = LocalMatrix->Rotation[1];
			rotation.z = LocalMatrix->Rotation[2];
		}
		inline void GetLocalQuaternion(physx::PxQuat& quaternion)
		{
			float4 buffer;

			buffer = Quaternion(
				LocalMatrix->Rotation[0],
				LocalMatrix->Rotation[1],
				LocalMatrix->Rotation[2]
			);

			quaternion.x = buffer.x;
			quaternion.y = buffer.y;
			quaternion.z = buffer.z;
			quaternion.w = buffer.w;
		}
		inline void GetLocalScale(physx::PxVec3& scale)
		{
			scale.x = LocalMatrix->Scale[0];
			scale.y = LocalMatrix->Scale[1];
			scale.z = LocalMatrix->Scale[2];
		}
		inline void GetLocalTransform(physx::PxTransform& transform)
		{
			float4 buffer;

			buffer = Quaternion(
				LocalMatrix->Rotation[0],
				LocalMatrix->Rotation[1],
				LocalMatrix->Rotation[2]
			);

			transform.p.x = LocalMatrix->Position[0];
			transform.p.y = LocalMatrix->Position[1];
			transform.p.z = LocalMatrix->Position[2];

			transform.q.x = buffer.x;
			transform.q.y = buffer.y;
			transform.q.z = buffer.z;
			transform.q.w = buffer.w;
		}

		// Get Final(World + Local) Transform
		inline void GetTransform(physx::PxTransform& transform)
		{
			float4 buffer;
			float3 Rotation;
			
			Rotation.x = (LocalMatrix->Rotation[0] + GlobalMatrix->Rotation[0]);
			Rotation.y = (LocalMatrix->Rotation[1] + GlobalMatrix->Rotation[1]);
			Rotation.z = (LocalMatrix->Rotation[2] + GlobalMatrix->Rotation[2]);

			ClampVector3(Rotation, 0.0f, 360.0f);

			buffer = Quaternion(Rotation);

			transform.p.x = LocalMatrix->Position[0] + GlobalMatrix->Position[0];
			transform.p.y = LocalMatrix->Position[1] + GlobalMatrix->Position[1];
			transform.p.z = LocalMatrix->Position[2] + GlobalMatrix->Position[2];

			transform.q.x = buffer.x;
			transform.q.y = buffer.y;
			transform.q.z = buffer.z;
			transform.q.w = buffer.w;
		}

		inline void SetPosition(float s, bool update = true)
		{
			LocalMatrix->Position[0] = s;
			LocalMatrix->Position[1] = s;
			LocalMatrix->Position[2] = s;

			bIsDirtyInstance = update;
		}
		inline void SetPosition(float x, float y, float z, bool update = true)
		{
			LocalMatrix->Position[0] = x;
			LocalMatrix->Position[1] = y;
			LocalMatrix->Position[2] = z;

			bIsDirtyInstance = update;
		}
		inline void SetPosition(float3& position, bool update = true)
		{
			LocalMatrix->Position[0] = position.x;
			LocalMatrix->Position[1] = position.y;
			LocalMatrix->Position[2] = position.z;

			bIsDirtyInstance = update;
		}
		inline void SetPosition(physx::PxVec3& position, bool update = true)
		{
			LocalMatrix->Position[0] = position.x;
			LocalMatrix->Position[1] = position.y;
			LocalMatrix->Position[2] = position.z;

			bIsDirtyInstance = update;
		}

		inline void SetRotation(float s, bool update = true)
		{
			LocalMatrix->Rotation[0] = s;
			LocalMatrix->Rotation[1] = s;
			LocalMatrix->Rotation[2] = s;

			bIsDirtyInstance = update;
		}
		inline void SetRotation(float x, float y, float z, bool update = true)
		{
			LocalMatrix->Rotation[0] = x;
			LocalMatrix->Rotation[1] = y;
			LocalMatrix->Rotation[2] = z;

			bIsDirtyInstance = update;
		}
		inline void SetRotation(float3& rotation, bool update = true)
		{
			LocalMatrix->Rotation[0] = rotation.x;
			LocalMatrix->Rotation[1] = rotation.y;
			LocalMatrix->Rotation[2] = rotation.z;

			bIsDirtyInstance = update;
		}
		inline void SetRotation(physx::PxVec3& rotation, bool update = true)
		{
			LocalMatrix->Rotation[0] = rotation.x;
			LocalMatrix->Rotation[1] = rotation.y;
			LocalMatrix->Rotation[2] = rotation.z;

			bIsDirtyInstance = update;
		}

		inline void SetScale(float s, bool update = true)
		{
			LocalMatrix->Scale[0] = s;
			LocalMatrix->Scale[1] = s;
			LocalMatrix->Scale[2] = s;

			bIsDirtyInstance = update;
		}
		inline void SetScale(float x, float y, float z, bool update = true)
		{
			LocalMatrix->Scale[0] = x;
			LocalMatrix->Scale[1] = y;
			LocalMatrix->Scale[2] = z;

			bIsDirtyInstance = update;
		}
		inline void SetScale(float3& scale, bool update = true)
		{
			LocalMatrix->Scale[0] = scale.x;
			LocalMatrix->Scale[1] = scale.y;
			LocalMatrix->Scale[2] = scale.z;

			bIsDirtyInstance = update;
		}
		inline void SetScale(physx::PxVec3& scale, bool update = true)
		{
			LocalMatrix->Scale[0] = scale.x;
			LocalMatrix->Scale[1] = scale.y;
			LocalMatrix->Scale[2] = scale.z;

			bIsDirtyInstance = update;
		}

		inline void SetGlobalPosition(float s)
		{
			GlobalMatrix->Position[0] = s;
			GlobalMatrix->Position[1] = s;
			GlobalMatrix->Position[2] = s;
		}
		inline void SetGlobalPosition(float x, float y, float z)
		{
			GlobalMatrix->Position[0] = x;
			GlobalMatrix->Position[1] = y;
			GlobalMatrix->Position[2] = z;
		}
		inline void SetGlobalPosition(float3& position)
		{
			GlobalMatrix->Position[0] = position.x;
			GlobalMatrix->Position[1] = position.y;
			GlobalMatrix->Position[2] = position.z;
		}
		inline void SetGlobalPosition(physx::PxVec3& position)
		{
			GlobalMatrix->Position[0] = position.x;
			GlobalMatrix->Position[1] = position.y;
			GlobalMatrix->Position[2] = position.z;
		}

		inline void SetGlobalRotation(float s)
		{
			GlobalMatrix->Rotation[0] = s;
			GlobalMatrix->Rotation[1] = s;
			GlobalMatrix->Rotation[2] = s;
		}
		inline void SetGlobalRotation(float x, float y, float z)
		{
			GlobalMatrix->Rotation[0] = x;
			GlobalMatrix->Rotation[1] = y;
			GlobalMatrix->Rotation[2] = z;
		}
		inline void SetGlobalRotation(float3& rotation)
		{
			GlobalMatrix->Rotation[0] = rotation.x;
			GlobalMatrix->Rotation[1] = rotation.y;
			GlobalMatrix->Rotation[2] = rotation.z;
		}
		inline void SetGlobalRotation(physx::PxVec3& rotation)
		{
			GlobalMatrix->Rotation[0] = rotation.x;
			GlobalMatrix->Rotation[1] = rotation.y;
			GlobalMatrix->Rotation[2] = rotation.z;
		}

		inline void SetGlobalScale(float s)
		{
			GlobalMatrix->Scale[0] = s;
			GlobalMatrix->Scale[1] = s;
			GlobalMatrix->Scale[2] = s;
		}
		inline void SetGlobalScale(float x, float y, float z)
		{
			GlobalMatrix->Scale[0] = x;
			GlobalMatrix->Scale[1] = y;
			GlobalMatrix->Scale[2] = z;
		}
		inline void SetGlobalScale(float3& scale)
		{
			GlobalMatrix->Scale[0] = scale.x;
			GlobalMatrix->Scale[1] = scale.y;
			GlobalMatrix->Scale[2] = scale.z;
		}
		inline void SetGlobalScale(physx::PxVec3& scale)
		{
			GlobalMatrix->Scale[0] = scale.x;
			GlobalMatrix->Scale[1] = scale.y;
			GlobalMatrix->Scale[2] = scale.z;
		}

		inline void SetTransform(
			float PX, float PY, float PZ,
			float RX, float RY, float RZ,
			float SX, float SY, float SZ, bool update = true
		) 
		{
			LocalMatrix->Position[0]	= PX;
			LocalMatrix->Position[1]	= PY;
			LocalMatrix->Position[2]	= PZ;

			LocalMatrix->Rotation[0]	= RX;
			LocalMatrix->Rotation[1]	= RY;
			LocalMatrix->Rotation[2]	= RZ;

			LocalMatrix->Scale[0]		= SX;
			LocalMatrix->Scale[1]		= SY;
			LocalMatrix->Scale[2]		= SZ;

			bIsDirtyInstance = update;
		}
		inline void SetTransform(
			float3& position,
			float3& rotation,
			float3& scale, 
			bool update = true
		)
		{
			LocalMatrix->Position[0]	= position.x;
			LocalMatrix->Position[1]	= position.y;
			LocalMatrix->Position[2]	= position.z;

			LocalMatrix->Rotation[0]	= rotation.x;
			LocalMatrix->Rotation[1]	= rotation.y;
			LocalMatrix->Rotation[2]	= rotation.z;

			LocalMatrix->Scale[0]		= scale.x;
			LocalMatrix->Scale[1]		= scale.y;
			LocalMatrix->Scale[2]		= scale.z;

			bIsDirtyInstance = update;
		}
		inline void SetTransform(
			physx::PxVec3& position,
			physx::PxVec3& rotation,
			physx::PxVec3& scale, 
			bool update = true
		)
		{
			LocalMatrix->Position[0]	= position.x;
			LocalMatrix->Position[1]	= position.y;
			LocalMatrix->Position[2]	= position.z;

			LocalMatrix->Rotation[0]	= rotation.x;
			LocalMatrix->Rotation[1]	= rotation.y;
			LocalMatrix->Rotation[2]	= rotation.z;

			LocalMatrix->Scale[0]		= scale.x;
			LocalMatrix->Scale[1]		= scale.y;
			LocalMatrix->Scale[2]		= scale.z;

			bIsDirtyInstance = update;
		}
		inline void SetTransform(
			physx::PxTransform& transform, 
			bool update = true
		)
		{
			// Quaternion => Euler
			float3 buffer = Euler(
								transform.q.x,
								transform.q.y,
								transform.q.z,
								transform.q.w
							);

			LocalMatrix->Position[0] = transform.p.x;
			LocalMatrix->Position[1] = transform.p.y;
			LocalMatrix->Position[2] = transform.p.z;

			LocalMatrix->Rotation[0] = buffer.x;
			LocalMatrix->Rotation[1] = buffer.y;
			LocalMatrix->Rotation[2] = buffer.z;

			LocalMatrix->Scale[0] = 1.0f;
			LocalMatrix->Scale[1] = 1.0f;
			LocalMatrix->Scale[2] = 1.0f;

			bIsDirtyInstance = update;
		}

		inline void SetVelocity(float x, float y, float z, bool update = true)
		{
			Velocity->x = x;
			Velocity->y = y;
			Velocity->z = z;

			bIsDirtyInstance = update;
		}
		inline void SetVelocity(float3& velocity, bool update = true)
		{
			Velocity->x = velocity.x;
			Velocity->y = velocity.y;
			Velocity->z = velocity.z;

			bIsDirtyInstance = update;
		}
		inline void SetVelocity(physx::PxVec3& velocity, bool update = true)
		{
			Velocity->x = velocity.x;
			Velocity->y = velocity.y;
			Velocity->z = velocity.z;

			bIsDirtyInstance = update;
		}

		inline void SetTorque(float x, float y, float z, bool update = true)
		{
			Torque->x = x;
			Torque->y = y;
			Torque->z = z;

			bIsDirtyInstance = update;
		}
		inline void SetTorque(float3& torque, bool update = true)
		{
			Torque->x = torque.x;
			Torque->y = torque.y;
			Torque->z = torque.z;

			bIsDirtyInstance = update;
		}
		inline void SetTorque(physx::PxVec3& torque, bool update = true)
		{
			Torque->x = torque.x;
			Torque->y = torque.y;
			Torque->z = torque.z;

			bIsDirtyInstance = update;
		}

		inline void SetAccumulate(float x, float y, float z, bool update = true)
		{
			Accumulate->x = x;
			Accumulate->y = y;
			Accumulate->z = z;

			bIsDirtyInstance = update;
		}
		inline void SetAccumulate(float3& accumulate, bool update = true)
		{
			Accumulate->x = accumulate.x;
			Accumulate->y = accumulate.y;
			Accumulate->z = accumulate.z;

			bIsDirtyInstance = update;
		}
		inline void SetAccumulate(physx::PxVec3& accumulate, bool update = true)
		{
			Accumulate->x = accumulate.x;
			Accumulate->y = accumulate.y;
			Accumulate->z = accumulate.z;

			bIsDirtyInstance = update;
		}

		inline void AddPosition(float x, float y, float z, bool update = true)
		{
			LocalMatrix->Position[0] += x;
			LocalMatrix->Position[1] += y;
			LocalMatrix->Position[2] += z;

			bIsDirtyInstance = update;
		}
		inline void AddPosition(float3& position, bool update = true)
		{
			LocalMatrix->Position[0] += position.x;
			LocalMatrix->Position[1] += position.y;
			LocalMatrix->Position[2] += position.z;

			bIsDirtyInstance = update;
		}
		inline void AddPosition(physx::PxVec3& position, bool update = true)
		{
			LocalMatrix->Position[0] += position.x;
			LocalMatrix->Position[1] += position.y;
			LocalMatrix->Position[2] += position.z;

			bIsDirtyInstance = update;
		}

		inline void AddRotation(float x, float y, float z, bool update = true)
		{
			LocalMatrix->Rotation[0] += x;
			LocalMatrix->Rotation[1] += y;
			LocalMatrix->Rotation[2] += z;

			bIsDirtyInstance = update;
		}
		inline void AddRotation(float3& rotation, bool update = true)
		{
			LocalMatrix->Rotation[0] += rotation.x;
			LocalMatrix->Rotation[1] += rotation.y;
			LocalMatrix->Rotation[2] += rotation.z;

			bIsDirtyInstance = update;
		}
		inline void AddRotation(physx::PxVec3& rotation, bool update = true)
		{
			LocalMatrix->Rotation[0] += rotation.x;
			LocalMatrix->Rotation[1] += rotation.y;
			LocalMatrix->Rotation[2] += rotation.z;

			bIsDirtyInstance = update;
		}

		inline void AddScale(float x, float y, float z, bool update = true)
		{
			LocalMatrix->Scale[0] += x;
			LocalMatrix->Scale[1] += y;
			LocalMatrix->Scale[2] += z;

			bIsDirtyInstance = update;
		}
		inline void AddScale(float3& scale, bool update = true)
		{
			LocalMatrix->Scale[0] += scale.x;
			LocalMatrix->Scale[1] += scale.y;
			LocalMatrix->Scale[2] += scale.z;

			bIsDirtyInstance = update;
		}
		inline void AddScale(physx::PxVec3& scale, bool update = true)
		{
			LocalMatrix->Scale[0] += scale.x;
			LocalMatrix->Scale[1] += scale.y;
			LocalMatrix->Scale[2] += scale.z;

			bIsDirtyInstance = update;
		}

		inline void AddVelocity(float x, float y, float z, bool update = true)
		{
			Velocity->x += x;
			Velocity->y += y;
			Velocity->z += z;

			bIsDirtyInstance = update;
		}
		inline void AddVelocity(float3& velocity, bool update = true)
		{
			Velocity->x += velocity.x;
			Velocity->y += velocity.y;
			Velocity->z += velocity.z;

			bIsDirtyInstance = update;
		}
		inline void AddVelocity(physx::PxVec3& velocity, bool update = true)
		{
			Velocity->x += velocity.x;
			Velocity->y += velocity.y;
			Velocity->z += velocity.z;

			bIsDirtyInstance = update;
		}

		inline void AddTorque(float x, float y, float z, bool update = true)
		{
			Torque->x += x;
			Torque->y += y;
			Torque->z += z;

			bIsDirtyInstance = update;
		}
		inline void AddTorque(float3& torque, bool update = true)
		{
			Torque->x += torque.x;
			Torque->y += torque.y;
			Torque->z += torque.z;

			bIsDirtyInstance = update;
		}
		inline void AddTorque(physx::PxVec3& torque, bool update = true)
		{
			Torque->x += torque.x;
			Torque->y += torque.y;
			Torque->z += torque.z;

			bIsDirtyInstance = update;
		}

		inline void AddAccumulate(float x, float y, float z, bool update = true)
		{
			Accumulate->x += x;
			Accumulate->y += y;
			Accumulate->z += z;

			bIsDirtyInstance = update;
		}
		inline void AddAccumulate(float3& accumulate, bool update = true)
		{
			Accumulate->x += accumulate.x;
			Accumulate->y += accumulate.y;
			Accumulate->z += accumulate.z;

			bIsDirtyInstance = update;
		}
		inline void AddAccumulate(physx::PxVec3& accumulate, bool update = true)
		{
			Accumulate->x += accumulate.x;
			Accumulate->y += accumulate.y;
			Accumulate->z += accumulate.z;

			bIsDirtyInstance = update;
		}
	};

public:
	Transform() {

	}

	~Transform() {

	}

	// Instance Information
public:
	std::string				    Name;

	UINT						ID_Count = 1;
	UINT						InstanceCount = 1;
	bool						InstanceDirty = false;

	std::unordered_map<std::string, transform*> InstanceBuffer;

};
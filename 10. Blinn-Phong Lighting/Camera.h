#pragma once
#include "Controller.h"

// Delta Time도 싱글톤 만들기
class Camera
{
private:
    float3 GlobalPosition = { 0.0f, 0.0f, 0.0f };
    float3 LocalPosition = { 0.0f, 0.0f, 0.0f };

    float4x4 GlobalCameraViewMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    float4x4 GlobalInvCameraViewMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    float4x4 LocalCameraViewMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    float4x4 LocalInvCameraViewMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    float CameraMoveSpeed = 100.0f;
    float CameraTurnSpeed = static_cast<float>(M_PI) * 30.0f;

    float DeltaTime = 0.0f;

private:
    bool IsMouseRClicked = false;

public:
    // 카메라의 기본 스케일 값은 1
    /*
        View Matrix =
        {
            X_x, Y_x, Z_x, P_x,
            X_y, Y_y, Z_y, P_y,
            X_z, Y_z, Z_z, P_z,
            0  , 0  , 0  , 1
        }
    */

    inline float3 GetGlobalPosition()
    {
        return GlobalPosition;
    }

    inline float3 GetLocalPosition()
    {
        return LocalPosition;
    }

    inline float4 GetPosition()
    {
        float4 Position;
        Position.x = GlobalPosition.x + LocalPosition.x;
        Position.y = GlobalPosition.y + LocalPosition.y;
        Position.z = GlobalPosition.z + LocalPosition.z;
        Position.w = 1.0f;

        return Position;
    }

    inline float3 GetLocalForwardVector()
    {
        float3 L = {
            LocalCameraViewMatrix.m[0][2],
            LocalCameraViewMatrix.m[1][2],
            LocalCameraViewMatrix.m[2][2],
        };

        L = normalise(L);

        return L;
    }

    inline float3 GetLocalUpVector()
    {
        float3 U = {
            LocalCameraViewMatrix.m[0][1],
            LocalCameraViewMatrix.m[1][1],
            LocalCameraViewMatrix.m[2][1],
        };

        U = normalise(U);

        return U;
    }

    inline float3 GetLocalRightVector()
    {
        float3 R = {
            LocalCameraViewMatrix.m[0][0],
            LocalCameraViewMatrix.m[1][0],
            LocalCameraViewMatrix.m[2][0],
        };

        R = normalise(R);

        return R;
    }

    inline void MoveLocalForward()
    {
        float3 L = {
            LocalCameraViewMatrix.m[0][2],
            LocalCameraViewMatrix.m[1][2],
            LocalCameraViewMatrix.m[2][2],
        };

        L = normalise(L);

        LocalPosition.x -= L.x * CameraMoveSpeed * DeltaTime;
        LocalPosition.y -= L.y * CameraMoveSpeed * DeltaTime;
        LocalPosition.z -= L.z * CameraMoveSpeed * DeltaTime;
    }
    inline void MoveLocalBackward()
    {
        float3 L = {
            LocalCameraViewMatrix.m[0][2],
            LocalCameraViewMatrix.m[1][2],
            LocalCameraViewMatrix.m[2][2],
        };

        L = normalise(L);

        LocalPosition.x += L.x * CameraMoveSpeed * DeltaTime;
        LocalPosition.y += L.y * CameraMoveSpeed * DeltaTime;
        LocalPosition.z += L.z * CameraMoveSpeed * DeltaTime;
    }

    inline void MoveLocalLeft()
    {
        float3 L = {
            LocalCameraViewMatrix.m[0][0],
            LocalCameraViewMatrix.m[1][0],
            LocalCameraViewMatrix.m[2][0],
        };

        L = normalise(L);

        LocalPosition.x -= LocalCameraViewMatrix.m[0][0] * CameraMoveSpeed * DeltaTime;
        LocalPosition.y -= LocalCameraViewMatrix.m[1][0] * CameraMoveSpeed * DeltaTime;
        LocalPosition.z -= LocalCameraViewMatrix.m[2][0] * CameraMoveSpeed * DeltaTime;
    }

    inline void MoveLocalRight()
    {
        LocalPosition.x += LocalCameraViewMatrix.m[0][0] * CameraMoveSpeed * DeltaTime;
        LocalPosition.y += LocalCameraViewMatrix.m[1][0] * CameraMoveSpeed * DeltaTime;
        LocalPosition.z += LocalCameraViewMatrix.m[2][0] * CameraMoveSpeed * DeltaTime;
    }

    inline void MoveLocalUp()
    {
        LocalPosition.x += LocalCameraViewMatrix.m[0][1] * CameraMoveSpeed * DeltaTime;
        LocalPosition.y += LocalCameraViewMatrix.m[1][1] * CameraMoveSpeed * DeltaTime;
        LocalPosition.z += LocalCameraViewMatrix.m[2][1] * CameraMoveSpeed * DeltaTime;
    }

    inline void MoveLocalDown()
    {
        LocalPosition.x -= LocalCameraViewMatrix.m[0][1] * CameraMoveSpeed * DeltaTime;
        LocalPosition.y -= LocalCameraViewMatrix.m[1][1] * CameraMoveSpeed * DeltaTime;
        LocalPosition.z -= LocalCameraViewMatrix.m[2][1] * CameraMoveSpeed * DeltaTime;
    }

    inline void LocalYaw(float delta)
    {
        float degree = delta * CameraTurnSpeed * DeltaTime;

        LocalCameraViewMatrix = LocalCameraViewMatrix * rotateYMat(DirectX::XMConvertToRadians(degree));
        LocalInvCameraViewMatrix = LocalInvCameraViewMatrix * rotateYMat(DirectX::XMConvertToRadians(-degree));
    }

    inline void LocalRoll(float delta)
    {
        float degree = delta * CameraTurnSpeed * DeltaTime;

        LocalCameraViewMatrix = LocalCameraViewMatrix * rotateZMat(DirectX::XMConvertToRadians(degree));
        LocalInvCameraViewMatrix = LocalInvCameraViewMatrix * rotateZMat(DirectX::XMConvertToRadians(-degree));
    }

    inline void LocalPitch(float delta)
    {
        float degree = delta * CameraTurnSpeed * DeltaTime;

        LocalCameraViewMatrix = LocalCameraViewMatrix * rotateXMat(DirectX::XMConvertToRadians(degree));
        LocalInvCameraViewMatrix = LocalInvCameraViewMatrix * rotateXMat(DirectX::XMConvertToRadians(-degree));
    }

    inline float3 GetGlobalForwardVector()
    {
        float3 L = {
            GlobalCameraViewMatrix.m[0][2],
            GlobalCameraViewMatrix.m[1][2],
            GlobalCameraViewMatrix.m[2][2],
        };

        L = normalise(L);

        return L;
    }

    inline float3 GetGlobalUpVector()
    {
        float3 U = {
            GlobalCameraViewMatrix.m[0][1],
            GlobalCameraViewMatrix.m[1][1],
            GlobalCameraViewMatrix.m[2][1],
        };

        U = normalise(U);

        return U;
    }

    inline float3 GetGlobalRightVector()
    {
        float3 R = {
            GlobalCameraViewMatrix.m[0][0],
            GlobalCameraViewMatrix.m[1][0],
            GlobalCameraViewMatrix.m[2][0],
        };

        R = normalise(R);

        return R;
    }

    inline void MoveGlobalForward()
    {
        float3 L = {
            GlobalCameraViewMatrix.m[0][2],
            GlobalCameraViewMatrix.m[1][2],
            GlobalCameraViewMatrix.m[2][2],
        };

        L = normalise(L);

        GlobalPosition.x -= L.x * CameraMoveSpeed * DeltaTime;
        GlobalPosition.y -= L.y * CameraMoveSpeed * DeltaTime;
        GlobalPosition.z -= L.z * CameraMoveSpeed * DeltaTime;
    }
    inline void MoveGlobalBackward()
    {
        float3 L = {
            GlobalCameraViewMatrix.m[0][2],
            GlobalCameraViewMatrix.m[1][2],
            GlobalCameraViewMatrix.m[2][2],
        };

        L = normalise(L);

        GlobalPosition.x += L.x * CameraMoveSpeed * DeltaTime;
        GlobalPosition.y += L.y * CameraMoveSpeed * DeltaTime;
        GlobalPosition.z += L.z * CameraMoveSpeed * DeltaTime;
    }

    inline void MoveGlobalLeft()
    {
        float3 L = {
            GlobalCameraViewMatrix.m[0][0],
            GlobalCameraViewMatrix.m[1][0],
            GlobalCameraViewMatrix.m[2][0],
        };

        L = normalise(L);

        GlobalPosition.x -= GlobalCameraViewMatrix.m[0][0] * CameraMoveSpeed * DeltaTime;
        GlobalPosition.y -= GlobalCameraViewMatrix.m[1][0] * CameraMoveSpeed * DeltaTime;
        GlobalPosition.z -= GlobalCameraViewMatrix.m[2][0] * CameraMoveSpeed * DeltaTime;
    }

    inline void MoveGlobalRight()
    {
        GlobalPosition.x += GlobalCameraViewMatrix.m[0][0] * CameraMoveSpeed * DeltaTime;
        GlobalPosition.y += GlobalCameraViewMatrix.m[1][0] * CameraMoveSpeed * DeltaTime;
        GlobalPosition.z += GlobalCameraViewMatrix.m[2][0] * CameraMoveSpeed * DeltaTime;
    }

    inline void MoveGlobalUp()
    {
        GlobalPosition.x += GlobalCameraViewMatrix.m[0][1] * CameraMoveSpeed * DeltaTime;
        GlobalPosition.y += GlobalCameraViewMatrix.m[1][1] * CameraMoveSpeed * DeltaTime;
        GlobalPosition.z += GlobalCameraViewMatrix.m[2][1] * CameraMoveSpeed * DeltaTime;
    }

    inline void MoveGlobalDown()
    {
        GlobalPosition.x -= GlobalCameraViewMatrix.m[0][1] * CameraMoveSpeed * DeltaTime;
        GlobalPosition.y -= GlobalCameraViewMatrix.m[1][1] * CameraMoveSpeed * DeltaTime;
        GlobalPosition.z -= GlobalCameraViewMatrix.m[2][1] * CameraMoveSpeed * DeltaTime;
    }

    inline void GlobalYaw(float delta)
    {
        float degree = delta * CameraTurnSpeed * DeltaTime;

        GlobalCameraViewMatrix = GlobalCameraViewMatrix * rotateYMat(DirectX::XMConvertToRadians(degree));
        GlobalInvCameraViewMatrix = GlobalInvCameraViewMatrix * rotateYMat(DirectX::XMConvertToRadians(-degree));
    }

    inline void GlobalRoll(float delta)
    {
        float degree = delta * CameraTurnSpeed * DeltaTime;

        GlobalCameraViewMatrix = GlobalCameraViewMatrix * rotateZMat(DirectX::XMConvertToRadians(degree));
        GlobalInvCameraViewMatrix = GlobalInvCameraViewMatrix * rotateZMat(DirectX::XMConvertToRadians(-degree));
    }

    inline void GlobalPitch(float delta)
    {
        float degree = delta * CameraTurnSpeed * DeltaTime;

        GlobalCameraViewMatrix = GlobalCameraViewMatrix * rotateXMat(DirectX::XMConvertToRadians(degree));
        GlobalInvCameraViewMatrix = GlobalInvCameraViewMatrix * rotateXMat(DirectX::XMConvertToRadians(-degree));
    }

    /*
        TranslationMat * RotationMat을
        RotationMat * TranslationMat으로 전환하면 Spring Arm이 됨.
    */

    // World To NDC
    inline float4x4 getViewMat()
    {
        return translationMat(-LocalPosition) * LocalInvCameraViewMatrix * translationMat(-GlobalPosition) * GlobalInvCameraViewMatrix;
    }

    // NDC To World
    inline float4x4 getInvViewMat()
    {
        return  GlobalCameraViewMatrix * translationMat(GlobalPosition) * LocalCameraViewMatrix * translationMat(LocalPosition);
    }

    void Update();
    void HandleUpdate();
};

class CameraPool {
private:
    static CameraPool gCameraPool;

    UINT mainCameraIDX;
    Camera cameras[10];

public:
    const UINT MaxCameraCount = 10;

public:
    static inline CameraPool& getInstance()
    {
        return gCameraPool;
    }

    Camera& getCamera()
    {
        return cameras[mainCameraIDX];
    }

    void setMainCamera(UINT camIndex)
    {
        if (camIndex < 10)
            mainCameraIDX = camIndex;
    }
};
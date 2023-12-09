#pragma once

#define _USE_MATH_DEFINES 
#include <math.h>

#include "fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")

#include <DirectXMath.h>

#pragma warning(push)
#pragma warning(disable:4201) // anonymous struct warning

struct float3
{
    float x, y, z;
};

union float4
{
    struct {
        float x, y, z, w;
    };
    struct {
        float3 xyz;
    };
};
#pragma warning(pop)

struct float3x3
{
    // We internally do 3x4 for HLSL-friendly alignment
    float m[3][4];
};

union float4x4
{
    float m[4][4];
    float4 cols[4];

    float4x4()
    {
        m[0][0] = 0.0f;
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[0][3] = 0.0f;

        m[1][0] = 0.0f;
        m[1][1] = 0.0f;
        m[1][2] = 0.0f;
        m[1][3] = 0.0f;

        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        m[2][2] = 0.0f;
        m[2][3] = 0.0f;

        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = 0.0f;
        m[3][3] = 0.0f;
    }
    float4x4(
        float m11, float m12, float m13, float m14, 
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44
        ) 
    {
        m[0][0] = m11;
        m[0][1] = m12;
        m[0][2] = m13;
        m[0][3] = m14;

        m[1][0] = m21;
        m[1][1] = m22;
        m[1][2] = m23;
        m[1][3] = m24;

        m[2][0] = m31;
        m[2][1] = m32;
        m[2][2] = m33;
        m[2][3] = m34;

        m[3][0] = m41;
        m[3][1] = m42;
        m[3][2] = m43;
        m[3][3] = m44;
    }

    inline float4 row(int i) { // Returns i-th row of matrix
        return { m[0][i], m[1][i], m[2][i], m[3][i] };
    }
    fbxsdk::FbxAMatrix toFbxAMatrix() {
        fbxsdk::FbxAMatrix fbxMatrix;

        fbxMatrix.mData[0].mData[0] = m[0][0];
        fbxMatrix.mData[0].mData[1] = m[0][1];
        fbxMatrix.mData[0].mData[2] = m[0][2];
        fbxMatrix.mData[0].mData[3] = m[0][3];

        fbxMatrix.mData[1].mData[0] = m[1][0];
        fbxMatrix.mData[1].mData[1] = m[1][1];
        fbxMatrix.mData[1].mData[2] = m[1][2];
        fbxMatrix.mData[1].mData[3] = m[1][3];

        fbxMatrix.mData[2].mData[0] = m[2][0];
        fbxMatrix.mData[2].mData[1] = m[2][1];
        fbxMatrix.mData[2].mData[2] = m[2][2];
        fbxMatrix.mData[2].mData[3] = m[2][3];

        fbxMatrix.mData[3].mData[0] = m[3][0];
        fbxMatrix.mData[3].mData[1] = m[3][1];
        fbxMatrix.mData[3].mData[2] = m[3][2];
        fbxMatrix.mData[3].mData[3] = m[3][3];

        return fbxMatrix;
    }
};

inline float degreesToRadians(float degs) {
    return degs * ((float)M_PI / 180.0f);
}

inline float length(float3 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline float length(float4 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z +v.w*v.w);
}

inline float dot(float4 a, float4 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

inline float3 operator* (float3 v, float f) {
    return {v.x*f, v.y*f, v.z*f};
}

inline float4 operator* (float4 v, float f) {
    return {v.x*f, v.y*f, v.z*f, v.w*f};
}

inline float3 operator* (float f, float3 v) {
    return { v.x * f, v.y * f, v.z * f };
}

inline float4 operator* (float f, float4 v) {
    return { v.x * f, v.y * f, v.z * f, v.w * f };
}

inline float3 normalise(float3 v) {
    return v * (1.f / length(v));
}

inline float4 normalise(float4 v) {
    return v * (1.f / length(v));
}

inline float3 cross(float3 a, float3 b) {
    return {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

inline float3 operator+= (float3 &lhs, float3 rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

inline float3 operator-= (float3 &lhs, float3 rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

inline float3 operator- (float3 v) {
    return {-v.x, -v.y, -v.z};
}

inline float4x4 scaleMat(float s) {
    return {
        s, 0, 0, 0,
        0, s, 0, 0,
        0, 0, s, 0,
        0, 0, 0, 1
    };
}

inline float4x4 scaleMat(float3 s) {
    return {
        s.x, 0, 0, 0,
        0, s.y, 0, 0,
        0, 0, s.z, 0,
        0, 0, 0, 1
    };
}

inline float4x4 rotateXMat(float rad) {
    float sinTheta = sinf(rad);
    float cosTheta = cosf(rad);
    return {
        1, 0, 0, 0,
        0, cosTheta, -sinTheta, 0,
        0, sinTheta, cosTheta, 0,
        0, 0, 0, 1
    };
}

inline float4x4 rotateYMat(float rad) {
    float sinTheta = sinf(rad);
    float cosTheta = cosf(rad);
    return {
        cosTheta, 0, sinTheta, 0,
        0, 1, 0, 0,
        -sinTheta, 0, cosTheta, 0,
        0, 0, 0, 1
    };
}

inline float4x4 rotateZMat(float rad) {
    float sinTheta = sinf(rad);
    float cosTheta = cosf(rad);
    return {
        cosTheta, -sinTheta, 0, 0,
        sinTheta, cosTheta, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

inline float4x4 rotateMat(float3 euler) {
    float sinB = sinf(euler.y);
    float cosB = cosf(euler.y);

    float sinA = sinf(euler.z);
    float cosA = cosf(euler.z);

    float sinC = sinf(euler.x);
    float cosC = cosf(euler.x);

    return {
        cosA * cosB, cosA * sinB * sinC - sinA * cosC, cosA * sinB * cosC + sinA * sinC, 0,
        sinA * cosB, sinA * sinB * sinC + cosA * cosC, sinA * sinB * cosC - cosA * sinC, 0,
        -sinB, cosB * sinC, cosB * cosC, 0,
        0, 0, 0, 1
    };
}

inline float4x4 rotateMat2Radian(float3 euler) {
    float toRadian = (3.14159265f / 180.0f);

    float sinB = sinf(euler.y * toRadian);
    float cosB = cosf(euler.y * toRadian);

    float sinA = sinf(euler.z * toRadian);
    float cosA = cosf(euler.z * toRadian);

    float sinC = sinf(euler.x * toRadian);
    float cosC = cosf(euler.x * toRadian);

    return {
        cosA * cosB, cosA * sinB * sinC - sinA * cosC, cosA * sinB * cosC + sinA * sinC, 0,
        sinA * cosB, sinA * sinB * sinC + cosA * cosC, sinA * sinB * cosC - cosA * sinC, 0,
        -sinB, cosB * sinC, cosB * cosC, 0,
        0, 0, 0, 1
    };
}

inline float4 Quaternion(float x, float y, float z) {
    float cr = cos(x * 0.5f);
    float sr = sin(x * 0.5f);
    float cp = cos(y * 0.5f);
    float sp = sin(y * 0.5f);
    float cy = cos(z * 0.5f);
    float sy = sin(z * 0.5f);

    float4 q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

inline float4 Quaternion(float3 euler) {
    float cr = cos(euler.x * 0.5f);
    float sr = sin(euler.x * 0.5f);
    float cp = cos(euler.y * 0.5f);
    float sp = sin(euler.y * 0.5f);
    float cy = cos(euler.z * 0.5f);
    float sy = sin(euler.z * 0.5f);

    float4 q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

inline float4x4 QuaternionMat(float4 q) {
    return {
        1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y - 2 * q.w * q.z, 2 * q.x * q.z + 2 * q.w * q.y, 0,
        2 * q.x * q.y + 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z - 2 * q.w * q.x, 0,
        2 * q.x * q.z - 2 * q.w * q.y, 2 * q.y * q.z + 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
        0, 0, 0, 1
    };
}

inline float4x4 Euler2QuaternionMat(float3 euler) {
    float cr = cos(euler.x * 0.5f);
    float sr = sin(euler.x * 0.5f);
    float cp = cos(euler.y * 0.5f);
    float sp = sin(euler.y * 0.5f);
    float cy = cos(euler.z * 0.5f);
    float sy = sin(euler.z * 0.5f);

    float4 q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return {
        1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y - 2 * q.w * q.z, 2 * q.x * q.z + 2 * q.w * q.y, 0,
        2 * q.x * q.y + 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z - 2 * q.w * q.x, 0,
        2 * q.x * q.z - 2 * q.w * q.y, 2 * q.y * q.z + 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
        0, 0, 0, 1
    };
}

// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
inline float3 Euler(float x, float y, float z, float w) {
    float3 angles;

    // roll (x-axis rotation)
    float sinr_cosp = 2 * (w * x + y * z);
    float cosr_cosp = 1 - 2 * (x * x + y * y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = std::sqrt(1 + 2 * (w * y - x * z));
    float cosp = std::sqrt(1 - 2 * (w * y - x * z));
    angles.y = 2 * std::atan2(sinp, cosp) - static_cast<float>(M_PI) / 2;

    // yaw (z-axis rotation)
    float siny_cosp = 2 * (w * z + x * y);
    float cosy_cosp = 1 - 2 * (y * y + z * z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
inline float3 Euler(float4 q) {
    float3 angles;

    // roll (x-axis rotation)
    float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    float cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles.y = 2 * std::atan2(sinp, cosp) - static_cast<float>(M_PI) / 2;

    // yaw (z-axis rotation)
    float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

inline float4x4 translationMat(float3 trans) {
    return {
        1, 0, 0, trans.x,
        0, 1, 0, trans.y,
        0, 0, 1, trans.z,
        0, 0, 0, 1
    };
}

inline float4x4 operator* (float4x4 a, float4x4 b) {
    return {
        dot(a.row(0), b.cols[0]),
        dot(a.row(1), b.cols[0]),
        dot(a.row(2), b.cols[0]),
        dot(a.row(3), b.cols[0]),
        dot(a.row(0), b.cols[1]),
        dot(a.row(1), b.cols[1]),
        dot(a.row(2), b.cols[1]),
        dot(a.row(3), b.cols[1]),
        dot(a.row(0), b.cols[2]),
        dot(a.row(1), b.cols[2]),
        dot(a.row(2), b.cols[2]),
        dot(a.row(3), b.cols[2]),
        dot(a.row(0), b.cols[3]),
        dot(a.row(1), b.cols[3]),
        dot(a.row(2), b.cols[3]),
        dot(a.row(3), b.cols[3])
    };
}

inline float3 operator+ (float3 v, float3 m) {
    return {
        v.x + m.x,
        v.y + m.y,
        v.z + m.z
    };
}

inline float3 operator- (float3 v, float3 m) {
    return {
        v.x - m.x,
        v.y - m.y,
        v.z - m.z
    };
}

inline float4 operator+ (float4 v, float4 m) {
    return {
        v.x + m.x,
        v.y + m.y,
        v.z + m.z,
        v.w + m.w
    };
}

inline float4 operator- (float4 v, float4 m) {
    return {
        v.x - m.x,
        v.y - m.y,
        v.z - m.z,
        v.w - m.w
    };
}

inline float3 operator* (float3 v, float4x4 m) {
    float4 buffer = { v.x, v.y, v.z, 1.0f };

    m.row(0);

    return {
        dot(buffer, m.row(0)),
        dot(buffer, m.row(1)),
        dot(buffer, m.row(2))
    };
}

inline float4 operator* (float4 v, float4x4 m) {
    return {
        dot(v, m.cols[0]),
        dot(v, m.cols[1]),
        dot(v, m.cols[2]),
        dot(v, m.cols[3])
    };
}

inline float4x4 transpose(float4x4 m) {
    return float4x4 {
        m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], 
        m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1], 
        m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2], 
        m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
    };
}

inline float3x3 float4x4ToFloat3x3(float4x4 m) {
    float3x3 result = {
        m.m[0][0], m.m[0][1], m.m[0][2], 0.0, 
        m.m[1][0], m.m[1][1], m.m[1][2], 0.0,
        m.m[2][0], m.m[2][1], m.m[2][2], 0.0
    };
    return result;
}

inline float4x4 inverse(float4x4 m) {
    DirectX::XMMATRIX mat = {
        m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
        m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
        m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
        m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3],
    };

    DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(mat);
    DirectX::XMMATRIX inv = DirectX::XMMatrixInverse(&det, mat);

    return {
        inv.r[0].m128_f32[0], inv.r[0].m128_f32[1], inv.r[0].m128_f32[2], inv.r[0].m128_f32[3],
        inv.r[1].m128_f32[0], inv.r[1].m128_f32[1], inv.r[1].m128_f32[2], inv.r[1].m128_f32[3],
        inv.r[2].m128_f32[0], inv.r[2].m128_f32[1], inv.r[2].m128_f32[2], inv.r[2].m128_f32[3],
        inv.r[3].m128_f32[0], inv.r[3].m128_f32[1], inv.r[3].m128_f32[2], inv.r[3].m128_f32[3],
    };
}

inline float4x4 EulerToMatrix(float x, float y, float z)
{
    return
        float4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosf(x), sinf(x), 0.0f,
            0.0f, -sinf(x), cosf(x), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            cosf(y), 0.0f, -sinf(y), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sinf(y), 0.0f, cosf(y), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            cosf(z), sinf(z), 0.0f, 0.0f,
            -sinf(z), cosf(z), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

}

inline float4x4 makeAffineMatrix(float* translation, float* rotation, float* scale)
{
    return
        float4x4(
            scale[0], 0.0f, 0.0f, 0.0f,
            0.0f, scale[1], 0.0f, 0.0f,
            0.0f, 0.0f, scale[2], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosf(rotation[0]), sinf(rotation[0]), 0.0f,
            0.0f, -sinf(rotation[0]), cosf(rotation[0]), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            cosf(rotation[1]), 0.0f, -sinf(rotation[1]), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sinf(rotation[1]), 0.0f, cosf(rotation[1]), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            cosf(rotation[2]), sinf(rotation[2]), 0.0f, 0.0f,
            -sinf(rotation[2]), cosf(rotation[2]), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            translation[0], translation[1], translation[2], 1
        );

}
inline float4x4 makeAffineMatrix(float3& translation, float3& rotation, float3& scale)
{
    return
        float4x4(
            scale.x, 0.0f, 0.0f, 0.0f,
            0.0f, scale.y, 0.0f, 0.0f,
            0.0f, 0.0f, scale.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosf(rotation.x), sinf(rotation.x), 0.0f,
            0.0f, -sinf(rotation.x), cosf(rotation.x), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            cosf(rotation.y), 0.0f, -sinf(rotation.y), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sinf(rotation.y), 0.0f, cosf(rotation.y), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            cosf(rotation.z), sinf(rotation.z), 0.0f, 0.0f,
            -sinf(rotation.z), cosf(rotation.z), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ) *
        float4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            translation.x, translation.y, translation.z, 1
        );

}

inline float4x4 makePerspectiveMat(float aspectRatio, float fovYRadians, float zNear, float zFar)
{
    // float yScale = 1 / tanf(0.5f * fovYRadians); 
    // NOTE: 1/tan(X) = tan(90degs - X), so we can avoid a divide
    // float yScale = tanf((0.5f * M_PI) - (0.5f * fovYRadians));
    float yScale = tanf(0.5f * ((float)M_PI - fovYRadians));
    float xScale = yScale / aspectRatio;
    float zRangeInverse = 1.f / (zNear - zFar);
    float zScale = zFar * zRangeInverse;
    float zTranslation = zFar * zNear * zRangeInverse;

    float4x4 result = {
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, zScale, zTranslation,
        0, 0, -1, 1
    };
    return result;
}

inline void ClampVector3(float& x, float& y, float& z, float min, float max)
{
    while (x < min)  x += max;
    while (max < x)  x -= max;

    while (y < min)  y += max;
    while (max < y)  y -= max;

    while (z < min)  z += max;
    while (max < z)  z -= max;
}

inline void ClampVector3(float3& vector, float min, float max)
{
    while (vector.x < min)  vector.x += max;
    while (max < vector.x)  vector.x -= max;

    while (vector.y < min)  vector.y += max;
    while (max < vector.y)  vector.y -= max;

    while (vector.z < min)  vector.z += max;
    while (max < vector.z)  vector.z -= max;
}

inline void ClampVector4(float4& vector, float min, float max)
{
    while (vector.x < min)  vector.x += max;
    while (max < vector.x)  vector.x -= max;

    while (vector.y < min)  vector.y += max;
    while (max < vector.y)  vector.y -= max;

    while (vector.z < min)  vector.z += max;
    while (max < vector.z)  vector.z -= max;

    vector.w = 1.0f;
}
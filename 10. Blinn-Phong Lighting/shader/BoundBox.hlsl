
cbuffer vsConstants : register(b0)
{
    float4x4 modelViewProj;
    float4x4 modelView;
    float3x3 normalMatrix;
    float4   cameraPosition;
};

struct DirectionalLight
{
    float4 ambientColor;

    float dummy;
    float ambientStrength;
    float specularStrength;
    float specularExponent;

    float4 dirEye; //NOTE: Direction *towards* the light
    float4x4 lightViewMatrix;
    float4x4 lightProjectionMatrix;
};

struct PointLight
{
    float4 ambientColor;

    float dummy;
    float ambientStrength;
    float specularStrength;
    float specularExponent;

    float4 posEye;
    float4x4 lightViewMatrix;
    float4x4 lightProjectionMatrix;
};

// Create Constant Buffer for our Blinn-Phong vertex shader
cbuffer fsConstants : register(b0)
{
    int2 dummy;
    int dirLightNum;
    int pointLightNum;
    DirectionalLight dirLight[150];
    PointLight       pointLights[150];
};

struct VS_Input {
    float3 pos      : POS;
    float3 norm     : NORM;
    float3 tan      : TANGENT;
    float2 uv0      : TEX0;
    float2 uv1      : TEX1;
    float2 uv2      : TEX2;
    float2 uv3      : TEX3;
    float3 localPosition : INSTANCE;
    float3 localRotation : INSTANCE1;
    float3 localScale    : INSTANCE2;
};

struct VS_Output {
    float4 pos       : SV_POSITION;
    float3 posEye    : POSITION;
    float3 normalEye : NORMAL;
    float2 uv0       : TEXCOORD0;
    float2 uv1       : TEXCOORD1;
    float2 uv2       : TEXCOORD2;
    float2 uv3       : TEXCOORD3;
};

Texture2D    mytexture : register(t0);
SamplerState mysampler : register(s0);

Texture2D    depthtexture : register(t1);
SamplerState depthsampler : register(s1);

VS_Output vs_main(VS_Input input)
{
    VS_Output output;

    float4 pos4 = float4(input.pos + input.localPosition, 1.0f);

    output.pos = mul(pos4, modelViewProj);
    output.posEye = mul(pos4, modelView).xyz;
    output.normalEye = mul(input.norm, normalMatrix);

    output.uv0 = input.uv0;
    output.uv1 = input.uv1;
    output.uv2 = input.uv2;
    output.uv3 = input.uv3;

    return output;
}

float4 ps_main(VS_Output input) : SV_Target
{  
    return normalize(input.pos);
}

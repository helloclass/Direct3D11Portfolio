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

cbuffer ScreenSizeBuffer : register(b1)
{
    float screenWidth;
    float3 padding;
};

Texture2D    shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct VS_Input {
    float3 pos              : POS;
    float3 norm             : NORM;
    float3 tan              : TANGENT;
    float2 uv0              : TEX0;
    float2 uv1              : TEX1;
    float2 uv2              : TEX2;
    float2 uv3              : TEX3;
    float3 localPosition    : INSTANCE;
    float3 localRotation    : INSTANCE1;
    float3 localScale       : INSTANCE2;
};

struct VS_Output
{
    float4 position  : SV_POSITION;
    float4 colorrrrr : COLOR;
    float2 tex       : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
VS_Output vs_main(VS_Input input)
{
    VS_Output output;
    float texelSize;

    output.position = float4(input.pos, 1.0f);
    output.tex = input.uv0;

    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ps_main(VS_Output input) : SV_TARGET
{
    float4 color = shaderTexture.Sample(SampleType, input.tex);

    return color;
}

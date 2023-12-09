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
    float screenHeight;
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
    float2 tex       : TEXCOORD0;
    float2 texelSize : TEXELSIZE;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
VS_Output vs_main(VS_Input input)
{
    VS_Output output;

    output.position = float4(input.pos, 1.0f);
    output.tex = input.uv0;

    output.tex.x = 1.0f - output.tex.x;

    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ps_main(VS_Output input) : SV_TARGET
{
    float4 color;
    float4 origin;

    float texelSize = 1.0f / 960.0f;

    color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float2 c1 = input.tex + float2(0.0f, 0.0f);
    float2 c2 = input.tex + float2(texelSize * -1.0f, 0.0f);
    float2 c3 = input.tex + float2(texelSize * -2.0f, 0.0f);
    float2 c4 = input.tex + float2(texelSize, 0.0f);
    float2 c5  = input.tex + float2(texelSize * 2.0f, 0.0f);

    float2 c6 = input.tex + float2(texelSize * -1.0f, texelSize * -2.0f);
    float2 c7  = input.tex + float2(texelSize * -1.0f, texelSize * -1.0f);
    float2 c8  = input.tex + float2(texelSize * -1.0f, 0.0f);
    float2 c9  = input.tex + float2(texelSize * -1.0f, texelSize);
    float2 c10 = input.tex + float2(texelSize * -1.0f, texelSize * 2.0f);

    float2 c11 = input.tex + float2(0.0f, texelSize * -2.0f);
    float2 c12 = input.tex + float2(0.0f, texelSize * -1.0f);
    float2 c13 = input.tex + float2(0.0f, 0.0f);
    float2 c14 = input.tex + float2(0.0f, texelSize);
    float2 c15 = input.tex + float2(0.0f, texelSize * 2.0f);

    float2 c16 = input.tex + float2(texelSize, texelSize * -2.0f);
    float2 c17 = input.tex + float2(texelSize, texelSize * -1.0f);
    float2 c18 = input.tex + float2(texelSize, 0.0f);
    float2 c19 = input.tex + float2(texelSize, texelSize);
    float2 c20 = input.tex + float2(texelSize, texelSize * 2.0f);

    float2 c21 = input.tex + float2(texelSize * 2.0f, texelSize * -2.0f);
    float2 c22 = input.tex + float2(texelSize * 2.0f, texelSize * -1.0f);
    float2 c23 = input.tex + float2(texelSize * 2.0f, 0.0f);
    float2 c24 = input.tex + float2(texelSize * 2.0f, texelSize);
    float2 c25 = input.tex + float2(texelSize * 2.0f, texelSize * 2.0f);

    float Weight;
    float Weight1;
    float Weight2;

    color += shaderTexture.Sample(SampleType, c1) * 0.1f;
    color += shaderTexture.Sample(SampleType, c2) * 0.1f;
    color += shaderTexture.Sample(SampleType, c3) * 0.1f;
    color += shaderTexture.Sample(SampleType, c4) * 0.1f;
    color += shaderTexture.Sample(SampleType, c5) * 0.1f;

    color += shaderTexture.Sample(SampleType, c6) * 0.05f;
    color += shaderTexture.Sample(SampleType, c7) * 0.05f;
    color += shaderTexture.Sample(SampleType, c8) * 0.05f;
    color += shaderTexture.Sample(SampleType, c9) * 0.05f;
    color += shaderTexture.Sample(SampleType, c10) * 0.05f;

    color += shaderTexture.Sample(SampleType, c11) * 0.0f;
    color += shaderTexture.Sample(SampleType, c12) * 0.0f;
    color += shaderTexture.Sample(SampleType, c13) * 0.0f;
    color += shaderTexture.Sample(SampleType, c14) * 0.0f;
    color += shaderTexture.Sample(SampleType, c15) * 0.0f;

    color += shaderTexture.Sample(SampleType, c16) * -0.05f;
    color += shaderTexture.Sample(SampleType, c17) * -0.05f;
    color += shaderTexture.Sample(SampleType, c18) * -0.05f;
    color += shaderTexture.Sample(SampleType, c19) * -0.05f;
    color += shaderTexture.Sample(SampleType, c20) * -0.05f;

    color += shaderTexture.Sample(SampleType, c21) * -0.1f;
    color += shaderTexture.Sample(SampleType, c22) * -0.1f;
    color += shaderTexture.Sample(SampleType, c23) * -0.1f;
    color += shaderTexture.Sample(SampleType, c24) * -0.1f;
    color += shaderTexture.Sample(SampleType, c25) * -0.1f;

    Weight1 = (color.x + color.y + color.z) / 3.0f;


    color += shaderTexture.Sample(SampleType, c1) * 0.1f;
    color += shaderTexture.Sample(SampleType, c6) * 0.1f;
    color += shaderTexture.Sample(SampleType, c11) * 0.1f;
    color += shaderTexture.Sample(SampleType, c16) * 0.1f;
    color += shaderTexture.Sample(SampleType, c21) * 0.1f;

    color += shaderTexture.Sample(SampleType, c2) * 0.05f;
    color += shaderTexture.Sample(SampleType, c6) * 0.05f;
    color += shaderTexture.Sample(SampleType, c12) * 0.05f;
    color += shaderTexture.Sample(SampleType, c17) * 0.05f;
    color += shaderTexture.Sample(SampleType, c22) * 0.05f;

    color += shaderTexture.Sample(SampleType, c3) * 0.0f;
    color += shaderTexture.Sample(SampleType, c8) * 0.0f;
    color += shaderTexture.Sample(SampleType, c13) * 0.0f;
    color += shaderTexture.Sample(SampleType, c18) * 0.0f;
    color += shaderTexture.Sample(SampleType, c23) * 0.0f;

    color += shaderTexture.Sample(SampleType, c4) * -0.05f;
    color += shaderTexture.Sample(SampleType, c9) * -0.05f;
    color += shaderTexture.Sample(SampleType, c14) * -0.05f;
    color += shaderTexture.Sample(SampleType, c19) * -0.05f;
    color += shaderTexture.Sample(SampleType, c24) * -0.05f;

    color += shaderTexture.Sample(SampleType, c5) * -0.1f;
    color += shaderTexture.Sample(SampleType, c10) * -0.1f;
    color += shaderTexture.Sample(SampleType, c15) * -0.1f;
    color += shaderTexture.Sample(SampleType, c20) * -0.1f;
    color += shaderTexture.Sample(SampleType, c25) * -0.1f;

    Weight2 = (color.x + color.y + color.z) / 3.0f;

    Weight = (Weight1 + Weight2) * 0.5f;

    return float4(Weight, Weight, Weight, 1.0f);
}

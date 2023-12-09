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

Texture2D    sobelShaderTexture : register(t1);
SamplerState sobelSampleType : register(s1);

Texture2D    depthShaderTexture : register(t2);
SamplerState depthSampleType : register(s2);

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

    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ps_main(VS_Output input) : SV_TARGET
{
    float4 color, sobelColor;
    float4 origin;

    float texelSize = 1.0f / 960.0f;

    color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    sobelColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

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

    float weight0;
    float weight1;
    float weight2;

    float dist = 1.0f - depthShaderTexture.Sample(depthSampleType, input.tex).r * 0.1f;

    //weight0 = 1.0f * dist;
    //weight1 = 1.0f - dist;
    //weight2 = 1.0f - dist;

    color += shaderTexture.Sample(SampleType, c1) * weight2;
    color += shaderTexture.Sample(SampleType, c2) * weight2;
    color += shaderTexture.Sample(SampleType, c3) * weight2;
    color += shaderTexture.Sample(SampleType, c4) * weight2;
    color += shaderTexture.Sample(SampleType, c5) * weight2;

    color += shaderTexture.Sample(SampleType, c6) * weight2;
    color += shaderTexture.Sample(SampleType, c7) * weight1;
    color += shaderTexture.Sample(SampleType, c8) * weight1;
    color += shaderTexture.Sample(SampleType, c9) * weight1;
    color += shaderTexture.Sample(SampleType, c10) * weight2;

    color += shaderTexture.Sample(SampleType, c11) * weight2;
    color += shaderTexture.Sample(SampleType, c12) * weight1;
    color += shaderTexture.Sample(SampleType, c13) * weight0;
    color += shaderTexture.Sample(SampleType, c14) * weight1;
    color += shaderTexture.Sample(SampleType, c15) * weight2;

    color += shaderTexture.Sample(SampleType, c16) * weight2;
    color += shaderTexture.Sample(SampleType, c17) * weight1;
    color += shaderTexture.Sample(SampleType, c18) * weight1;
    color += shaderTexture.Sample(SampleType, c19) * weight1;
    color += shaderTexture.Sample(SampleType, c20) * weight2;

    color += shaderTexture.Sample(SampleType, c21) * weight2;
    color += shaderTexture.Sample(SampleType, c22) * weight2;
    color += shaderTexture.Sample(SampleType, c23) * weight2;
    color += shaderTexture.Sample(SampleType, c24) * weight2;
    color += shaderTexture.Sample(SampleType, c25) * weight2;

    color.a = 1.0f;

    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c1) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c2) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c3) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c4) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c5) * weight2;

    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c6) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c7) * weight1;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c8) * weight1;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c9) * weight1;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c10) * weight2;

    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c11) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c12) * weight1;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c13) * weight0;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c14) * weight1;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c15) * weight2;

    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c16) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c17) * weight1;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c18) * weight1;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c19) * weight1;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c20) * weight2;

    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c21) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c22) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c23) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c24) * weight2;
    sobelColor += sobelShaderTexture.Sample(sobelSampleType, c25) * weight2;

    sobelColor.a = 1.0f;

    origin = 
        shaderTexture.Sample(SampleType, input.tex) +
        sobelShaderTexture.Sample(sobelSampleType, input.tex);

    //return origin * 0.8f + (color + sobelColor) * 0.2f;

    return origin * dist;
}

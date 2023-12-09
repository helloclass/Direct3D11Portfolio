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

    float4 pos4 = float4(input.pos, 1.0f);

    float4x4 localMat = float4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );

    float4x4 localScaleMat = float4x4(
        input.localScale.x, 0.0f, 0.0f, 0.0f,
        0.0f, input.localScale.y, 0.0f, 0.0f,
        0.0f, 0.0f, input.localScale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );

    localMat = mul(localMat, localScaleMat);

    float rx = radians(input.localRotation.x);
    float ry = radians(input.localRotation.y);
    float rz = radians(input.localRotation.z);

    float4x4 localRollMat =
        float4x4(
            cos(rz), sin(rz), 0.0f, 0.0f,
            -sin(rz), cos(rz), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            );
    float4x4 localYawMat =
        float4x4(
            cos(ry), 0.0f, -sin(ry), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sin(ry), 0.0f, cos(ry), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            );
    float4x4 localPitchMat =
        float4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cos(rx), sin(rx), 0.0f,
            0.0f, -sin(rx), cos(rx), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            );

    localMat = mul(localMat, localRollMat);
    localMat = mul(localMat, localYawMat);
    localMat = mul(localMat, localPitchMat);

    float4x4 localTransformationMat =
        float4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            input.localPosition.x, input.localPosition.y, input.localPosition.z, 1.0f
            );

    localMat = mul(localMat, localTransformationMat);
    pos4 = mul(pos4, localMat);

    output.pos = mul(pos4, modelViewProj);
    output.posEye = mul(float4(input.pos, 1.0f), modelView).xyz;
    output.normalEye = mul(input.norm, normalMatrix);

    output.uv0 = input.uv0;
    output.uv1 = input.uv1;
    output.uv2 = input.uv2;
    output.uv3 = input.uv3;

    return output;
}

float4 ps_main(VS_Output input) : SV_Target
{
    float4 diffuse = mytexture.Sample(mysampler, input.uv0);
    float3 diffuseColor = diffuse.xyz;

    float3 fragToCamDir = normalize(-input.posEye);

    clip(diffuse.w - 0.05f);

    // Directional Light
    int i = 0;
    float3 dirLightIntensity = float3(0, 0, 0);

    for (i = 0; i < dirLightNum; ++i)
    {
        float3 lightDirEye = dirLight[i].dirEye.xyz;
        float3 lightColor = dirLight[i].ambientColor.xyz;

        float3 iAmbient = dirLight[i].ambientStrength;

        float diffuseFactor = 
            max(0.0, dot(input.normalEye, lightDirEye));
        float3 iDiffuse = diffuseFactor;

        float3 halfwayEye = normalize(fragToCamDir + lightDirEye);
        float specularFactor = 
            max(0.0, dot(halfwayEye, input.normalEye));
        float3 iSpecular = 
            dirLight[i].specularStrength * 
            pow(specularFactor, 2 * dirLight[i].specularExponent);

        dirLightIntensity +=
            (iAmbient + iDiffuse + iSpecular) *
            lightColor;
    }

    // Point Light
    float3 pointLightIntensity = float3(0,0,0);

    for(i = 0; i < pointLightNum; ++i)
    {
        float3 lightDirEye = pointLights[i].posEye.xyz - input.posEye;
        float inverseDistance = 1.0f / length(lightDirEye);
        lightDirEye *= inverseDistance; //normalise

        float3 iAmbient = pointLights[i].ambientStrength;

        float diffuseFactor = max(0.0, dot(input.normalEye, lightDirEye));
        float3 iDiffuse = diffuseFactor;

        float3 halfwayEye = normalize(fragToCamDir + lightDirEye);
        float specularFactor = max(0.0, dot(halfwayEye, input.normalEye));
        float3 iSpecular = 
            pointLights[i].specularStrength * pow(specularFactor, 2* pointLights[i].specularExponent);

        pointLightIntensity += 
            (iAmbient + iDiffuse + iSpecular) * 
            pointLights[i].ambientColor.xyz * 
            inverseDistance;
    }

    float3 result = (dirLightIntensity + pointLightIntensity) * diffuseColor;
  
    return float4(result, 1.0f);
}

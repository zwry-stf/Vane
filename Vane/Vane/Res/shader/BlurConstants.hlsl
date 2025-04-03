#pragma once
// SharedConstants.hlsl
static const int samples = 40;
static const int LOD = 1;
static const int sLOD = (1 << LOD);
static const float sigma = samples * 0.25;
static const float gaussianNormalization = 1.0 / (6.28318530718 * sigma * sigma);

// Gaussian function
float gaussian(float2 i)
{
    float2 d = i / sigma;
    return exp(-0.5 * dot(d, d)) * gaussianNormalization;
}

// Structure for pixel shader input
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// Constant buffer for shader parameters
cbuffer BlurConstants : register(b0)
{
    float2 iResolution;
    int BlurEnabled;
    int BlurMenuOnly;

    float4 MenuPos;
    
    float iTime;
    float Animation;
    float NoiseScale;
    float Rounding;

    float ShadowSize;
    float ShadowAlpha;

    // Background Animation
    float TURB_AMP;
    float TURB_NUM;

    float TURB_SPEED;
    float TURB_FREQ;
    float TURB_EXP;

    float pad;

    float4 BackgroundColor;
    float4 MenuBackgroundColor;
    float4 AnimationColor;
};

Texture2D inputTexture : register(t0);
SamplerState inputSampler : register(s0);
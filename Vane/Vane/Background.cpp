#include <vector>
#include <random>
#include "Vane.h"
#include <DirectXMath.h>
#include <D3Dcompiler.h>

inline static const char* quadVSCode = R"(
struct VSInput {
    float2 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct VSOutput {
    float4 position : SV_Position;
    float2 uv       : TEXCOORD0;
};

VSOutput main(VSInput input) 
{
    VSOutput output;
    output.position = float4(input.pos, 0.0f, 1.0f);
    output.uv = input.uv;
    return output;
}
)";

// Just Vertical Blur
inline static const char* blurPSCode2 = R"(
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

// Texture and sampler declarations
Texture2D inputTexture : register(t0);
SamplerState inputSampler : register(s0);

// Constant buffer for shader parameters
cbuffer BlurConstants : register(b0)
{
    float2 iResolution;
    float Animation;
    float iTime;

    float NoiseScale;
    int BlurEnabled;
    int BlurMenuOnly;
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

    float4 MenuPos;

    float4 BackgroundColor;
    float4 MenuBackgroundColor;
    float4 AnimationColor;
};

// Structure for pixel shader input
struct PSInput {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};


// Custom smoothstep function
float smoothstep(float edge0, float edge1, float x)
{
    float t = saturate((x - edge0) / (edge1 - edge0));
    return t * t * (3.0 - 2.0 * t);
}

// Main pixel shader function
float4 main(PSInput input) : SV_TARGET
{
    float2 uv = input.uv;
    float2 fragCoord = uv * iResolution;

    int s = samples / sLOD;

    // Determine if the fragment is within the menu region
    bool posInMenu = fragCoord.x >= MenuPos.x - (float)s && fragCoord.x <= (MenuPos.x + MenuPos.z + (float)s) &&
                   fragCoord.y >= MenuPos.y - (float)s && fragCoord.y <= (MenuPos.y + MenuPos.w + (float)s);
    bool inMenu = BlurMenuOnly == 0 || posInMenu;

    float3 blurredColor = float3(0.0, 0.0, 0.0);

    // Compute blurred color if blur is enabled and fragment is in the menu area
    float mask = 0.f;
    if (BlurEnabled != 0 && inMenu)
    {
        float totalWeight = 0.0;
        float halfSamples = samples * 0.5;
        for (int yi = 0; yi < s; yi++)
        {
            float2 d = float2(0.0, yi) * sLOD - halfSamples;
            float weight = gaussian(d);
            float2 offset = d * (1.0 / iResolution);
            blurredColor += weight * inputTexture.SampleLevel(inputSampler, uv + offset, LOD).xyz;
            totalWeight += weight;
        }
        blurredColor /= totalWeight;
        mask = 1.f;
    }

    return float4(blurredColor, mask);
}
)";

inline static const char* blurPSCode = R"(
// Shader constants
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

// Texture and sampler declarations
Texture2D inputTexture : register(t0);
SamplerState inputSampler : register(s0);

// Constant buffer for shader parameters
cbuffer BlurConstants : register(b0)
{
    float2 iResolution;
    float Animation;
    float iTime;

    float NoiseScale;
    int BlurEnabled;
    int BlurMenuOnly;
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

    float4 MenuPos;

    float4 BackgroundColor;
    float4 MenuBackgroundColor;
    float4 AnimationColor;
};

// Structure for pixel shader input
struct PSInput {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

// Helper functions for noise generation
float3 mod289(float3 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float2 mod289(float2 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float3 permute(float3 x)
{
    return mod289(((x * 34.0) + 10.0) * x);
}

// Simplex noise function (snoise)
float snoise(float2 v)
{
    const float4 C = float4(0.211324865405187,   // (3.0 - sqrt(3.0)) / 6.0
                              0.366025403784439,   // 0.5 * (sqrt(3.0) - 1.0)
                             -0.577350269189626,   // -1.0 + 2.0 * C.x
                              0.024390243902439);  // 1.0 / 41.0

    float2 i = floor(v + dot(v, float2(C.y, C.y)));
    float2 x0 = v - i + dot(i, float2(C.x, C.x));

    float2 i1 = (x0.x > x0.y) ? float2(1.0, 0.0) : float2(0.0, 1.0);
    float4 x12 = float4(x0.x, x0.y, x0.x, x0.y) + float4(C.x, C.x, C.z, C.z);
    x12.xy -= i1;

    i = mod289(i);
    float3 p = permute( permute( i.y + float3(0.0, i1.y, 1.0) )
                      + i.x + float3(0.0, i1.x, 1.0) );

    float3 m = max(0.5 - float3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0);
    m = m * m;
    m = m * m;

    float3 x = 2.0 * frac(p * C.w) - 1.0;
    float3 h = abs(x) - 0.5;
    float3 ox = floor(x + 0.5);
    float3 a0 = x - ox;

    float3 g;
    g.x = a0.x * x0.x + h.x * x0.y;
    g.y = a0.y * x12.x + h.y * x12.y;
    g.z = a0.z * x12.z + h.z * x12.w;

    return 130.0 * dot(m, g);
}

// Custom smoothstep function
float smoothstep(float edge0, float edge1, float x)
{
    float t = saturate((x - edge0) / (edge1 - edge0));
    return t * t * (3.0 - 2.0 * t);
}

// Main pixel shader function
float4 main(PSInput input) : SV_TARGET
{
    float2 uv = input.uv;
    float2 fragCoord = uv * iResolution;

    // Determine if the fragment is within the menu region
    bool posInMenu = fragCoord.x >= MenuPos.x && fragCoord.x <= (MenuPos.x + MenuPos.z) &&
                   fragCoord.y >= MenuPos.y && fragCoord.y <= (MenuPos.y + MenuPos.w);
    bool inMenu = BlurMenuOnly == 0 || posInMenu;

    float3 blurredColor = float3(0.0, 0.0, 0.0);

    // Compute blurred color if blur is enabled and fragment is in the menu area
    if (BlurEnabled != 0 && inMenu)
    {
        float totalWeight = 0.0;
        int s = samples / sLOD;
        float halfSamples = samples * 0.5;
        for (int xi = 0; xi < s; xi++)
        {
            float2 d = float2(xi, 0.0) * sLOD - halfSamples;
            float weight = gaussian(d);
            float2 offset = d * (1.0 / iResolution);
            blurredColor += weight * inputTexture.SampleLevel(inputSampler, uv + offset, LOD).xyz;
            totalWeight += weight;
        }
        blurredColor /= totalWeight;
    }

    // Apply noise and rounded corners if within the menu area
    float mask = 0.0;
    if (BlurMenuOnly != 0 && BlurEnabled != 0)
    {
        if (posInMenu)
        {
            float2 halfSize = MenuPos.zw * 0.5;
            float2 center = MenuPos.xy + halfSize;
            float2 q = abs(fragCoord - center) - (halfSize - Rounding);
            float dist = length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - Rounding + 0.5;
            float aa = fwidth(dist);
            mask = smoothstep(0.0, aa, -dist);
            mask = clamp(mask, 0.0, 1.0);
            blurredColor *= mask;
        }
    }
    else if (BlurEnabled != 0)
        mask = 1.0;

    // Blend blurred color with background color
    float4 blendedColor;
    blendedColor.rgb = lerp(blurredColor, BackgroundColor.rgb, BackgroundColor.a);
    blendedColor.a   = BackgroundColor.a + mask * (1.0 - BackgroundColor.a);

    // Apply shadow and background effects
    {
        float4 shadowColor = MenuBackgroundColor;
        float2 halfSize = MenuPos.zw * 0.5;
        float2 center = MenuPos.xy + halfSize;
        float2 q = abs(fragCoord - center) - (halfSize - Rounding);
        float dist = length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - Rounding + 0.5;

        if (dist <= 0.0)
        {
            // We are inside the window
            shadowColor.a *= ShadowAlpha;
            blendedColor.rgb = lerp(blendedColor.rgb, shadowColor.rgb, shadowColor.a);
            blendedColor.a   = shadowColor.a + blendedColor.a * (1.0 - shadowColor.a);
        }
        else
        {
            // Use Gaussian falloff for the shadow weight
            float shadowWeight = exp(-0.5 * (dist * dist) / (ShadowSize * ShadowSize));
            shadowWeight = clamp(shadowWeight, 0.0, 1.0);

            if (shadowWeight > 0.003)
            {
                shadowColor.a *= shadowWeight * ShadowAlpha;
                blendedColor.rgb = lerp(blendedColor.rgb, shadowColor.rgb, shadowColor.a);
                blendedColor.a   = shadowColor.a + blendedColor.a * (1.0 - shadowColor.a);
            }
        }

        // Apply background
        float aa = fwidth(dist);
        mask = smoothstep(0.0, aa, -dist);
        mask = clamp(mask, 0.0, 1.0);

        shadowColor = MenuBackgroundColor;
        shadowColor.a *= 0.25 * mask;
        blendedColor.rgb = lerp(blendedColor.rgb, shadowColor.rgb, shadowColor.a);
        blendedColor.a   = shadowColor.a + blendedColor.a * (1.0 - shadowColor.a);

        // Apply Background Animation
        if (posInMenu) 
        {
            float2 p = 2.0 * ((fragCoord * 2.0) - iResolution) / iResolution.y;
            float freq = TURB_FREQ;
            float2x2 rot = float2x2( 0.6, -0.8, 0.8, 0.6 );

            // Loop through turbulence octaves.
            // (Note: we use a fixed loop count of 10)
            for (float i = 0; i < TURB_NUM; i++)
            {
                // Compute phase: using the rotated y component.
                float phase = freq * (p.x * rot._21 + p.y * rot._22) + TURB_SPEED * iTime + i;
                // Use the first row of rot for offset.
                p += TURB_AMP * float2(rot._11, rot._12) * sin(phase) / freq;
                // Update rotation matrix and frequency.
                float2x2 rot2 = float2x2( 0.6, -0.8, 0.8, 0.6 );
                rot = mul(rot, rot2);
                freq *= TURB_EXP;
            }
            // Blue-yellow gradient.
            float3 col = 0.5f * exp(0.1f * p.x * float3(-1.0, 0.0, 2.0));
            // Vary brightness.
            float d = dot(cos(p * 3.0), sin(float2(-p.y, p.x) * 3.0 * 0.618)) + 2.0;
            col /= d;
            // Exponential tonemap.
            col = 1.0 - exp(-col);

            float avarage = col.x * 0.2126f + col.y * 0.7152f + col.z * 0.0722f;

            // Blend Color
            float4 Color = AnimationColor * avarage;
            Color.a *= mask;
            blendedColor.rgb = lerp(blendedColor.rgb, Color.rgb, Color.a);
            blendedColor.a   = Color.a + blendedColor.a * (1.0 - Color.a);
        }

        // Noise
        if (BlurEnabled != 0)
        {
            float noise = snoise(fragCoord) * 0.1;
            if (BlurMenuOnly != 0)
                blendedColor += noise * NoiseScale * mask;
            else
                blendedColor += noise * NoiseScale;
            blendedColor = clamp(blendedColor, 0.0, 1.0);
        }
    }

    // Final blend with animation factor
    return lerp(float4(0.0, 0.0, 0.0, 0.0), blendedColor, Animation);
}
)";

bool Vane::Background::Init()
{
    long hr = S_OK;

    hr = renderer.g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&m_pBackBuffer));
    if (FAILED(hr))
    {
        Errors::Set("Failed to get backbuffer");
        return false;
    }

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = renderer.g_backBufferDesc.Width;
    texDesc.Height = renderer.g_backBufferDesc.Height;
    texDesc.MipLevels = renderer.g_backBufferDesc.MipLevels;
    texDesc.ArraySize = 1;
    texDesc.Format = renderer.g_backBufferDesc.Format;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    hr = renderer.g_pd3dDevice->CreateTexture2D(&texDesc, nullptr, &m_pOffscreenTexture);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create offscreen texture");
        return false;
    }

    hr = renderer.g_pd3dDevice->CreateShaderResourceView(m_pOffscreenTexture, nullptr,
        &m_pOffscreenSRV);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create shader resource view");
        return false;
    }

    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    hr = renderer.g_pd3dDevice->CreateTexture2D(&texDesc, nullptr, &m_p1PassTexture);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create Shader Blur texture");
        return false;
    }

    hr = renderer.g_pd3dDevice->CreateShaderResourceView(m_p1PassTexture, nullptr,
        &m_p1PassSRV);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create shader resource view 2");
        return false;
    }

    hr = renderer.g_pd3dDevice->CreateRenderTargetView(m_p1PassTexture, nullptr, &m_p1PassRTV);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create vertical blur RTV");
        return false;
    }

    ID3DBlob* vsBlob = 0;
    ID3DBlob* psBlob = 0;
    ID3DBlob* errorBlob = 0;

    // --- Vertex Shader --- 
    hr = D3DCompile(quadVSCode, strlen(quadVSCode), nullptr, nullptr, nullptr,
        "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (vsBlob)
            vsBlob->Release();
        if (errorBlob)
            Errors::Set("Failed to compile vertex shader: " + std::string((char*)errorBlob->GetBufferPointer()));
        else
            Errors::Set("Failed to compile vertex shader");
        if (errorBlob)
            errorBlob->Release();
        return false;
    }
    hr = renderer.g_pd3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        nullptr, &m_pQuadVS);
    if (FAILED(hr))
    {
        if (vsBlob) vsBlob->Release();
        Errors::Set("Failed to create vertex shader");
        return false;
    }

    // Define the input layout for the quad vertices.
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(XyVec4, x),
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(XyVec4, z),
          D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    hr = renderer.g_pd3dDevice->CreateInputLayout(layoutDesc, 2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_pInputLayout);
    if (vsBlob) vsBlob->Release();
    if (FAILED(hr))
    {
        Errors::Set("Failed to create input layout");
        return false;
    }

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = renderer.g_pd3dDevice->CreateBlendState(&blendDesc, &m_pBlendState);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create blend state.");
        return false;
    }

    // --- Pixel Shader (Blur) ---
    hr = D3DCompile(blurPSCode, strlen(blurPSCode), nullptr, nullptr, nullptr,
        "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (psBlob)
            psBlob->Release();
        if (errorBlob)
            Errors::Set((char*)errorBlob->GetBufferPointer());
        else
            Errors::Set("Failed to compile pixel shader");
        if (errorBlob)
            errorBlob->Release();
        return false;
    }
    if (errorBlob)
        errorBlob->Release();

    hr = renderer.g_pd3dDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
        nullptr, &m_pBlurPS);
    if (psBlob) psBlob->Release();
    if (FAILED(hr))
    {
        Errors::Set("Failed to create pixel shader.");
        return false;
    }


    // --- Pixel Shader 2 (Blur Vertical) ---
    hr = D3DCompile(blurPSCode2, strlen(blurPSCode2), nullptr, nullptr, nullptr,
        "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (psBlob)
            psBlob->Release();
        if (errorBlob)
            Errors::Set((char*)errorBlob->GetBufferPointer());
        else
            Errors::Set("Failed to compile pixel shader");
        if (errorBlob)
            errorBlob->Release();
        return false;
    }
    if (errorBlob)
        errorBlob->Release();

    hr = renderer.g_pd3dDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
        nullptr, &m_pBlurPSV);
    if (psBlob) psBlob->Release();
    if (FAILED(hr))
    {
        Errors::Set("Failed to create pixel shader 2.");
        return false;
    }

    // Create constant buffer for blur parameters.
    if (!ConstantBuffer.Init(renderer.g_pd3dDevice))
    {
        Errors::Set("Failed to create constant buffer.");
        return false;
    }


    // Vertex Buffer
    // x and y are pos, z and w are uv
    XyVec4 vertices[] = {
        { -1.0f, -1.0f, 0.0f, 1.0f },
        { -1.0f,  1.0f, 0.0f, 0.0f },
        { 1.0f, -1.0f, 1.0f, 1.0f },
        { 1.0f,  1.0f, 1.0f, 0.0f },
    };

    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    hr = renderer.g_pd3dDevice->CreateBuffer(&vbDesc, &initData, &m_pQuadVB);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create vertex buffer.");
        return false;
    }

    // Index Buffer
    unsigned int indices[] = {
        0, 1, 2, // First triangle
        2, 1, 3  // Second triangle
    };

    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.ByteWidth = sizeof(indices);
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    initData = {};
    initData.pSysMem = indices;

    hr = renderer.g_pd3dDevice->CreateBuffer(&ibDesc, &initData, &m_pQuadIB);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create vertex buffer.");
        return false;
    }

    // Sample State
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = renderer.g_pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);
    if (FAILED(hr))
    {
        Errors::Set("Failed to create sampler state.");
        return false;
    }

    return true;
}

void Vane::Background::Render()
{
    D3D11_VIEWPORT viewport = {};
    viewport.Width = renderer.data.display_size.x;
    viewport.Height = renderer.data.display_size.y;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    renderer.g_pd3dDeviceContext->RSSetViewports(1, &viewport);

    if (renderer.g_backBufferDesc.SampleDesc.Count > 1)
        renderer.g_pd3dDeviceContext->ResolveSubresource(m_pOffscreenTexture, 0, m_pBackBuffer, 0, renderer.g_backBufferDesc.Format);
    else
        renderer.g_pd3dDeviceContext->CopyResource(m_pOffscreenTexture, m_pBackBuffer);

    // --- First Pass: Vertical Blur ---
    renderer.g_pd3dDeviceContext->OMSetRenderTargets(1, &m_p1PassRTV, nullptr);
    renderer.g_pd3dDeviceContext->RSSetViewports(1, &viewport);

    float clearColor[4] = { 0, 0, 0, 0 };
    renderer.g_pd3dDeviceContext->ClearRenderTargetView(m_p1PassRTV, clearColor);

    uint32_t stride = sizeof(XyVec4);
    uint32_t offset = 0;
    renderer.g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pQuadVB, &stride, &offset);
    renderer.g_pd3dDeviceContext->IASetIndexBuffer(m_pQuadIB, DXGI_FORMAT_R32_UINT, 0);
    renderer.g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer.g_pd3dDeviceContext->IASetInputLayout(m_pInputLayout);

    renderer.g_pd3dDeviceContext->VSSetShader(m_pQuadVS, nullptr, 0);
    renderer.g_pd3dDeviceContext->PSSetShader(m_pBlurPSV, nullptr, 0);

    renderer.g_pd3dDeviceContext->PSSetShaderResources(0, 1, &m_pOffscreenSRV);
    renderer.g_pd3dDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

    ConstantBuffer->iResolution = renderer.data.display_size;
    ConstantBuffer->Animation = animation;
    ConstantBuffer->iTime += renderer.data.delta_time;
    ConstantBuffer->MenuPos = XyVec4(x, y, w, h);
    ConstantBuffer->Rounding = Style::Rounding;
    ConstantBuffer->MenuBackgroundColor = Style::Background;
    ConstantBuffer.Update(renderer.g_pd3dDeviceContext);
    renderer.g_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);

    float blendFactor[4] = { 0, 0, 0, 0 };
    uint32_t sampleMask = 0xffffffff;
    renderer.g_pd3dDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, sampleMask);

    renderer.g_pd3dDeviceContext->DrawIndexed(6, 0, 0);

    // --- First Pass: Horizontal Blur and Menu Background ---
    renderer.g_pd3dDeviceContext->OMSetRenderTargets(1, &renderer.g_pd3dRenderTargetView, nullptr);
    renderer.g_pd3dDeviceContext->RSSetViewports(1, &viewport);

    renderer.g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pQuadVB, &stride, &offset);
    renderer.g_pd3dDeviceContext->IASetIndexBuffer(m_pQuadIB, DXGI_FORMAT_R32_UINT, 0);
    renderer.g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer.g_pd3dDeviceContext->IASetInputLayout(m_pInputLayout);

    renderer.g_pd3dDeviceContext->VSSetShader(m_pQuadVS, nullptr, 0);
    renderer.g_pd3dDeviceContext->PSSetShader(m_pBlurPS, nullptr, 0);

    renderer.g_pd3dDeviceContext->PSSetShaderResources(0, 1, &m_p1PassSRV);
    renderer.g_pd3dDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

    renderer.g_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
    renderer.g_pd3dDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, sampleMask);

    renderer.g_pd3dDeviceContext->DrawIndexed(6, 0, 0);
}

void Vane::Background::Destroy()
{
    if (m_pOffscreenTexture) m_pOffscreenTexture->Release();
    if (m_pOffscreenSRV) m_pOffscreenSRV->Release();
    if (m_pBackBuffer) m_pBackBuffer->Release();
    if (m_pQuadVS) m_pQuadVS->Release();
    if (m_pBlurPS) m_pBlurPS->Release();
    if (m_pInputLayout) m_pInputLayout->Release();
    if (m_pQuadVB) m_pQuadVB->Release();
    if (m_pQuadIB) m_pQuadIB->Release();
    if (m_pSamplerState) m_pSamplerState->Release();
    if (m_pBlendState) m_pBlendState->Release();

    ConstantBuffer.Destroy();
}
#include "BlurConstants.hlsl"

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
    const float4 C = float4(0.211324865405187, // (3.0 - sqrt(3.0)) / 6.0
                              0.366025403784439, // 0.5 * (sqrt(3.0) - 1.0)
                             -0.577350269189626, // -1.0 + 2.0 * C.x
                              0.024390243902439); // 1.0 / 41.0

    float2 i = floor(v + dot(v, float2(C.y, C.y)));
    float2 x0 = v - i + dot(i, float2(C.x, C.x));

    float2 i1 = (x0.x > x0.y) ? float2(1.0, 0.0) : float2(0.0, 1.0);
    float4 x12 = float4(x0.x, x0.y, x0.x, x0.y) + float4(C.x, C.x, C.z, C.z);
    x12.xy -= i1;

    i = mod289(i);
    float3 p = permute(permute(i.y + float3(0.0, i1.y, 1.0))
                      + i.x + float3(0.0, i1.x, 1.0));

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
        float s = float(samples) / float(sLOD);
        float halfSamples = float(s) * 0.5;
        for (float xi = 0; xi < s; xi++)
        {
            float2 d = float2(xi - halfSamples, 0.0) * sLOD;
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
    blendedColor.a = BackgroundColor.a + mask * (1.0 - BackgroundColor.a);

    // Apply shadow and background effects
    {
        float4 shadowColor = MenuBackgroundColor;
        float2 halfSize = MenuPos.zw * 0.5;
        float2 center = MenuPos.xy + halfSize;
        float2 q = abs(fragCoord - center) - (halfSize - Rounding);
        float dist = length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - Rounding + 0.5;
        dist += 2.0; // Looks better on the shadows
        if (dist <= 0.0)
        {
            // We are inside the window
            shadowColor.a *= ShadowAlpha;
            blendedColor.rgb = lerp(blendedColor.rgb, shadowColor.rgb, shadowColor.a);
            blendedColor.a = shadowColor.a + blendedColor.a * (1.0 - shadowColor.a);
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
                blendedColor.a = shadowColor.a + blendedColor.a * (1.0 - shadowColor.a);
            }
        }
        
        dist -= 2.0; // Restore
        // Apply background
        float aa = fwidth(dist);
        mask = smoothstep(0.0, aa, -dist);
        mask = clamp(mask, 0.0, 1.0);

        shadowColor = MenuBackgroundColor;
        shadowColor.a *= 0.25 * mask;
        blendedColor.rgb = lerp(blendedColor.rgb, shadowColor.rgb, shadowColor.a);
        blendedColor.a = shadowColor.a + blendedColor.a * (1.0 - shadowColor.a);

        // Apply Background Animation
        if (posInMenu)
        {
            float2 p = 2.0 * ((fragCoord * 2.0) - iResolution) / iResolution.y;
            float freq = TURB_FREQ;
            float2x2 rot = float2x2(0.6, -0.8, 0.8, 0.6);

            // Loop through turbulence octaves.
            // (Note: we use a fixed loop count of 10)
            for (float i = 0; i < TURB_NUM; i++)
            {
                // Compute phase: using the rotated y component.
                float phase = freq * (p.x * rot._21 + p.y * rot._22) + TURB_SPEED * iTime + i;
                // Use the first row of rot for offset.
                p += TURB_AMP * float2(rot._11, rot._12) * sin(phase) / freq;
                // Update rotation matrix and frequency.
                float2x2 rot2 = float2x2(0.6, -0.8, 0.8, 0.6);
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
            blendedColor.a = Color.a + blendedColor.a * (1.0 - Color.a);
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
#include "BlurConstants.hlsl"

// Main pixel shader function
float4 main(PSInput input) : SV_Target
{
    float2 uv = input.uv;
    float2 fragCoord = uv * iResolution;

    int s = samples / sLOD;
    float fs = float(s);

    // Determine if the fragment is within the menu region
    bool posInMenu = fragCoord.x >= MenuPos.x - fs && fragCoord.x <= (MenuPos.x + MenuPos.z + fs) && // We need the pixels left and right for horizontal avarage
                   fragCoord.y >= MenuPos.y && fragCoord.y <= (MenuPos.y + MenuPos.w);
    bool inMenu = BlurMenuOnly == 0 || posInMenu;

    float3 blurredColor = float3(0.0, 0.0, 0.0);

    // Compute blurred color if blur is enabled and fragment is in the menu area
    float mask = 0.f;
    if (BlurEnabled != 0 && inMenu)
    {
        float totalWeight = 0.0;
        float halfSamples = fs * 0.5;
        for (int yi = 0; yi < s; yi++)
        {
            float2 d = float2(0.0, yi - halfSamples) * sLOD;
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
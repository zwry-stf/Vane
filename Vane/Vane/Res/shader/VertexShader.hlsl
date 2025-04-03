struct VSInput
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.pos, 0.0f, 1.0f);
    output.uv = input.uv;
    return output;
}
#include<VSOut.hlsli>
cbuffer transformations : register(b0)
{
    matrix MVP;
    float3 color;
}

struct VSIn
{
    float3 pos : POSITION;
};

VSOut main(VSIn i)
{
    VSOut o;
    o.pos = mul(MVP, float4(i.pos, 1.0f));
    o.col = float4(color, 1.0f);
    return o;
}

#include<VSColorOut.hlsli>
cbuffer transformations : register(b0)
{
    matrix MVP;
}

struct VSIn
{
    float3 pos : POSITION;
    float3 col : COLOR;
};

VSColorOut main(VSIn i)
{
    VSColorOut o;
    o.pos = mul(MVP, float4(i.pos, 1.0f));
    o.col = float4(i.col, 1.0f);
    return o;
}

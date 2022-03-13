#include<VSOut.hlsli>
cbuffer transformations : register(b0)
{
    matrix MVP;
}

struct VSIn
{
    float3 pos : POSITION;
};

VSOut main(VSIn i)
{
    VSOut o;
    o.pos = mul(MVP, float4(i.pos, 1.0f));
    o.col = float4(1.0f,0.0f,0.0f, 1.0f);
    return o;
}

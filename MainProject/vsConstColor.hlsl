cbuffer transformations : register(b0)
{
    matrix MVP;
}

struct VSConstColorIn
{
    float3 pos : SV_POSITION;
};

struct VSConstColorOut
{
    float4 pos : SV_POSITION;
};


VSConstColorOut main(VSConstColorIn i)
{
    VSConstColorOut o;
    o.pos = mul(MVP, float4(i.pos, 1.0f));
    return o;
}


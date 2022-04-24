
cbuffer transformations : register(b0)
{
    float3 color;
}

float4 main() : SV_TARGET
{
    //return float4(1, 1, 1, 1);
    return float4(color, 1.0f);
}

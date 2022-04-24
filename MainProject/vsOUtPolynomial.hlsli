
struct VSIn
{
    float4 a : SV_POSITION;
    float3 b : B;
    float3 c : C;
    float3 d : D;
};

struct VSOut
{
    float4 a : SV_POSITION;
    float3 b : B;
    float3 c : C;
    float3 d : D;
};

struct GSOutput
{
    float4 pos : SV_POSITION;
};

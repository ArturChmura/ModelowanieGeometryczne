
struct VSIn
{
    float4 x : SV_POSITION;
    float4 y : Y;
    float4 z : Z;
    int SIZE : SIZE;
};

struct GSIn
{
    float4 x : SV_POSITION;
    float4 y : Y;
    float4 z : Z;
    int SIZE : SIZE;
};

struct GSOutput
{
    float4 pos : SV_POSITION;
};
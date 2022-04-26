
struct VSBezierIn
{
    float4 x : SV_POSITION;
    float4 y : Y;
    float4 z : Z;
    int SIZE : SIZE;
};

struct GSBezierIn
{
    float4 x : SV_POSITION;
    float4 y : Y;
    float4 z : Z;
    int SIZE : SIZE;
};

struct GSBezierOutput
{
    float4 pos : SV_POSITION;
};
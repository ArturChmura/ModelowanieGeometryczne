
struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION1;
    float3 norm : NORMAL;
    float3 viewVec : VIEW;
};
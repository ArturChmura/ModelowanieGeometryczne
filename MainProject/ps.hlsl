#include<VSOut.hlsli>

float4 main(VSOut i) : SV_TARGET
{
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
    return i.col;
}

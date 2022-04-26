#include<VSColorOut.hlsli>

float4 main(VSColorOut i) : SV_TARGET
{
    return i.col;
}

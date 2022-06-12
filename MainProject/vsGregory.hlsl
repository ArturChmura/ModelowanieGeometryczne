#include "gregoryHeader.hlsli"

cbuffer transformations : register(b0)
{
    matrix MVP;
    
    static float4 p4[3];
    static float4 dU4[3];
    static float4 dV4[3];
    static float4 dUV4[3];
    static float4 dVU4[3];
}

float4 GetHermitVector(float u)
{
    return float4(
        2 * u * u * u - 3 * u * u + 1,
         u * u * u - 2 * u * u + u,
        -2 * u * u * u + 3 * u * u,
        u * u * u - u * u
        );
}

static float p[4][3] = (float[4][3]) p4;
static float dU[4][3] = (float[4][3]) dU4;
static float dV[4][3] = (float[4][3]) dV4;
static float dUV[4][3] = (float[4][3]) dUV4;
static float dVU[4][3] = (float[4][3]) dVU4;


GregoryOut main(GregoryIn input)
{
    float u = input.uv.x;
    float v = input.uv.y;
    
    float4 Hu = GetHermitVector(u);
    float4 Hv = GetHermitVector(v);
    
    float result[3];
    for (int i = 0; i < 3; ++i)
    {
        float4 T =
        {
            (u * dUV[0][i] + v * dVU[0][i]) / (u + v), (u * dUV[1][i] + (1 - v) * dVU[1][i]) / (u + (1 - v)),
            ((1 - u) * dUV[2][i] + v * dVU[2][i]) / ((1 - u) + v), ((1 - u) * dUV[3][i] + (1 - v) * dVU[3][i]) / ((1 - u) + (1 - v)),
        };
    
        matrix P =
        {
            p[0][i], p[1][i], dV[0][i], dV[1][i],
            p[2][i], p[3][i], dV[2][i], dV[3][i],
            dU[0][i], dU[1][i], T[0], T[1],
            dU[2][i], dU[3][i], T[2], T[3]
        };
      
        float4 HuP = mul(Hu, P);
        float HuPHv = mul(HuP, Hv);
        result[i] = HuPHv;

    }
      
    float4 p = { result[0], result[1], result[2], 1 };
    
    
    GregoryOut output = (GregoryOut) 0;
    output.pos = mul(MVP, p);
    
    return output;
}
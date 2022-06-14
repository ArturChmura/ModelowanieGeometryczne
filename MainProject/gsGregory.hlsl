#include "gregoryHeader.hlsli"

cbuffer transformations : register(b0)
{
    matrix MVP;
    
    float4 p4[3];
    float4 dU4[3];
    float4 dV4[3];
    float4 dUV4[3];
    float4 dVU4[3];
    int horizontalSlices;
    int verticalSlices;
}

float4 GetHermitVector(float u)
{
    return float4(
        2 * u * u * u - 3 * u * u + 1,
        -2 * u * u * u + 3 * u * u,
        u * u * u - 2 * u * u + u,
        u * u * u - u * u
        );
}

static float3 p[4] = (float[4][3]) p4;
static float3 dU[4] = (float[4][3]) dU4;
static float3 dV[4] = (float[4][3]) dV4;
static float3 dUV[4] = (float[4][3]) dUV4;
static float3 dVU[4] = (float[4][3]) dVU4;

[maxvertexcount(256)]
void main(
	point GregoryIn input[1],
	inout LineStream<GregoryOut> output
)
{
    float uu = input[0].uv.x;
    bool flip = input[0].uv.y > 0.5;
    float4 Huu = GetHermitVector(uu);
    
    int slices = flip ? verticalSlices : horizontalSlices;
    float step = 1.0f / slices;
    for (float vv = 0.0f; vv <= 1.f + step / 2; vv += step)
    {
        float4 Hvv = GetHermitVector(vv);
        float u = flip ? vv : uu;
        float v = flip ? uu : vv;
        
        float4 Hv = flip ? Huu : Hvv;
        float4 Hu = flip ? Hvv : Huu;
        float3 result;
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
            result[i] = v == 0.0f ? Hu[1] : p[2][i];
            float HuPHv = mul(HuP, Hv);
            result[i] = HuPHv;
        }
        
        GregoryOut element = (GregoryOut) 0;
        element.pos = mul(MVP, float4(result, 1));
    
        output.Append(element);
    }
    output.RestartStrip();
    
}
#include "gregoryHeader.hlsli"
#include "DeCasteljeu.hlsli"
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
    
    int slices = flip ? verticalSlices : horizontalSlices;
    float step = 1.0f / slices;
    for (float vv = 0.0f; vv <= 1.f + step / 2; vv += step)
    {
        float u = flip ? vv : uu;
        float v = flip ? uu : vv;
        
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
                p[0][i], dV[0][i], dV[1][i], p[1][i],
                dU[0][i], T[0], T[1], dU[1][i],
                dU[2][i], T[2], T[3], dU[3][i],
                p[2][i], dV[2][i], dV[3][i], p[3][i]
            };
            
            float coefXYZ[4];
            for (int j = 0; j < 4; ++j)
            {
                float coefX[4] =
                {
                    P[j][0],
                    P[j][1],
                    P[j][2],
                    P[j][3],
                };
              
                coefXYZ[j] = DeCasteljeu(coefX, u, 4);
            }
            
            result[i] = DeCasteljeu(coefXYZ, v, 4);
        }
        
        GregoryOut element = (GregoryOut) 0;
        element.pos = mul(MVP, float4(result, 1));
    
        output.Append(element);
    }
    output.RestartStrip();
    
}
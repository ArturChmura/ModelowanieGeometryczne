#include "surfaceBezierStructs.hlsli"
#include "DeBoor.hlsli"
#include "GetUV.hlsli"

sampler samp : register(s0);
Texture2D filterTexture : register(t0);
cbuffer transformations : register(b0)
{
    matrix MVP;
    float4 x4[4];
    float4 y4[4];
    float4 z4[4];
    int slices;
    int flipped;
    int rowIndex;
    int columnIndex;
    int rowCount;
    int columnCount;
    int filter;
}

static float x[16] = (float[16]) x4;
static float y[16] = (float[16]) y4;
static float z[16] = (float[16]) z4;

[maxvertexcount(256)]
void main(
	point SurfaceBezierIn input[1],
	inout LineStream<GSSurfaceBezierOutput> output
)
{
    float step = 1.0f / slices;
    
    float coefXYZ[3][4] = (float[3][4]) 0;
 
    for (int j = 0; j < 4; ++j)
    {

        float coefX[4] =
        {
            x[j  + 0* 4],
            x[j  + 1* 4],
            x[j  + 2* 4],
            x[j + 3 * 4],
        };
        float coefY[4] =
        {
            y[j + 0* 4],
            y[j + 1* 4],
            y[j + 2* 4],
            y[j + 3 * 4],
        };
        float coefZ[4] =
        {
            z[j  + 0* 4],
            z[j  + 1* 4],
            z[j  + 2* 4],
            z[j  + 3 * 4],
        };
        coefXYZ[0][j] = DeBoor(coefX, input[0].uv.x);
        coefXYZ[1][j] = DeBoor(coefY, input[0].uv.x);
        coefXYZ[2][j] = DeBoor(coefZ, input[0].uv.x);
    }
    
        
    
    for (float t = 0.0f; t <= 1.0f + step / 2; t += step)
    {
        if (filter)
        {
            float2 uv = GetUV(input[0].uv.x, t, flipped, rowIndex, columnIndex, rowCount, columnCount);
            float3 norm = filterTexture.SampleLevel(samp, uv, 0);
      
            if (norm.x < 0.5)
            {
                continue;
            }
        }
        float xx = DeBoor(coefXYZ[0], t);
        float yy = DeBoor(coefXYZ[1], t);
        float zz = DeBoor(coefXYZ[2], t);
        
        GSSurfaceBezierOutput element = (GSSurfaceBezierOutput) 0;
        element.pos = mul(MVP, float4(xx, yy, zz, 1.0f));
        output.Append(element);
    }
    
    output.RestartStrip();
}
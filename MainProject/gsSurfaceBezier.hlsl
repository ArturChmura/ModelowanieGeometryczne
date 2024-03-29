#include "surfaceBezierStructs.hlsli"
#include "DeCasteljeu.hlsli"
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
        coefXYZ[0][j] = DeCasteljeu(coefX, input[0].uv.x, 4);
        coefXYZ[1][j] = DeCasteljeu(coefY, input[0].uv.x, 4);
        coefXYZ[2][j] = DeCasteljeu(coefZ, input[0].uv.x, 4);
    }
    
        
    for (float t = 0.0f; t <= 1.0f + step / 2; t += step)
    {
        if(filter)
        {
            float2 uv = GetUV(input[0].uv.x, t, flipped, rowIndex, columnIndex, rowCount, columnCount);
            float2 flipped = float2(uv.y, uv.x); // zamiana, bo tekstura ma x poziomo
            float3 norm = filterTexture.SampleLevel(samp, flipped, 0);
      
            if (norm.x < 0.5)
            {
                output.RestartStrip();
                continue;
            }
        }
        
        float xx = DeCasteljeu(coefXYZ[0], t, 4);
        float yy = DeCasteljeu(coefXYZ[1], t, 4);
        float zz = DeCasteljeu(coefXYZ[2], t, 4);
        
        GSSurfaceBezierOutput element = (GSSurfaceBezierOutput) 0;
        element.pos = mul(MVP, float4(xx, yy, zz, 1.0f));
        output.Append(element);
        
      
    }
    output.RestartStrip();
}
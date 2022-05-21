#include "surfaceBezierStructs.hlsli"
#include "DeBoor.hlsli"

cbuffer transformations : register(b0)
{
    matrix MVP;
    float4 x4[4];
    float4 y4[4];
    float4 z4[4];
    int slices;
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
            x[j * 4 + 0],
            x[j * 4 + 1],
            x[j * 4 + 2],
            x[j * 4 + 3],
        };
        float coefY[4] =
        {
            y[j * 4 + 0],
            y[j * 4 + 1],
            y[j * 4 + 2],
            y[j * 4 + 3],
        };
        float coefZ[4] =
        {
            z[j * 4 + 0],
            z[j * 4 + 1],
            z[j * 4 + 2],
            z[j * 4 + 3],
        };
        coefXYZ[0][j] = DeBoor(coefX, input[0].uv.x);
        coefXYZ[1][j] = DeBoor(coefY, input[0].uv.x);
        coefXYZ[2][j] = DeBoor(coefZ, input[0].uv.x);
    }
    
        
    
    for (float t = 0.0f; t <= 1.0f + step / 2; t += step)
    {
        float xx = DeBoor(coefXYZ[0], t);
        float yy = DeBoor(coefXYZ[1], t);
        float zz = DeBoor(coefXYZ[2], t);
        
        GSSurfaceBezierOutput element = (GSSurfaceBezierOutput) 0;
        element.pos = mul(MVP, float4(xx, yy, zz, 1.0f));
        output.Append(element);
    }
    
    output.RestartStrip();
}
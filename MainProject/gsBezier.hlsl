#include "bezierStructs.hlsli"

cbuffer transformations : register(b0)
{
    matrix MVP;
    int2 screenSize;
}

float DeCasteljeu(float coef[4], float t, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int currentSize = n - i;
        for (int j = 0; j < currentSize - 1; j++)
        {
            coef[j] = coef[j] * (1 - t) + coef[j + 1] * t;
        }
    }
    float value = coef[0];
    
    return value;
}

int CalculateSlicesCount(GSIn i)
{
    int n = i.SIZE;
    float4 points[4];
    float2 screenPositions[4];
    points[0] = float4(i.x.x, i.y.x, i.z.x, 1.0f);
    points[1] = float4(i.x.y, i.y.y, i.z.y, 1.0f);
    points[2] = float4(i.x.z, i.y.z, i.z.z, 1.0f);
    points[3] = float4(i.x.w, i.y.w, i.z.w, 1.0f);
    
    for (int i = 0; i < n; ++i)
    {
        points[i] = mul(MVP, points[i]);
        points[i] /= points[i].w;
        screenPositions[i] = float2(
            (points[i].x + 1) * screenSize.x / 2.0f,
            (points[i].y + 1) * screenSize.y / 2.0f
        );
    }
    
    float area = 0.0f;
    for (int i = 0; i < n-1; ++i)
    {
        float l = length(screenPositions[i] - screenPositions[i + 1]);
        l = min(l, screenSize.x * 2);
        area += l;
    }
    int slices = min(max((int) area, 1), 255);
    return slices;
}

[maxvertexcount(256)]
void main(
	point GSIn input[1],
	inout LineStream<GSOutput> output
)
{
    int slices = CalculateSlicesCount(input[0]);
    float step = 1.0f / slices;
	
    for (float t = 0; t <= 1.0f; t += step)
    {
        float coefX[4] =
        {
            input[0].x.x,
            input[0].x.y,
            input[0].x.z,
            input[0].x.w
        };
        float x = DeCasteljeu(coefX, t, input[0].SIZE);
        float coefY[4] =
        {
            input[0].y.x,
            input[0].y.y,
            input[0].y.z,
            input[0].y.w
        };
        float y = DeCasteljeu(coefY, t, input[0].SIZE);
        float coefZ[4] =
        {
            input[0].z.x,
            input[0].z.y,
            input[0].z.z,
            input[0].z.w
        };
        float z = DeCasteljeu(coefZ, t, input[0].SIZE);
        
        GSOutput element;
        element.pos = mul(MVP, float4(x, y, z, 1.0f));
        output.Append(element);
    }
    
    output.RestartStrip();
}
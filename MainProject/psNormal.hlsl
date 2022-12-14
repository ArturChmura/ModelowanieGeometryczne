#include "normalHeader.hlsli"

texture2D heightMap : register(t0);
texture2D colorMap : register(t1);

sampler samp : register(s0);


static const float3 lightPositions[1] =
{
    float3(0, 100, 0)
};
static const float3 ambientColor = float3(0.5f, 0.5f, 0.5f);
static const float3 lightColor = float3(1.0f, 1.0f, 1.0f);
static const float kd = 0.5, ks = 0.2f, m = 10.0f;
static const float mh = 100.0f;


cbuffer cbWorld : register(b0) //Vertex Shader constant buffer slot 0
{
    int gridWidthCount;
    int gridLengthCount;
    
    float widthSize;
    float lengthSize;
};

float4 main(VSOutput input) : SV_TARGET
{
    float3 viewVec = normalize(input.viewVec);
    float3 normal = normalize(input.norm);
    
    float xf = (input.worldPos.x + widthSize / 2) / widthSize;
    float yf = (input.worldPos.z + lengthSize / 2) / lengthSize;
    float2 tex = float2(xf, yf);
    
    float3 surfaceColor = colorMap.Sample(samp, tex);
    
    
    if(input.worldPos.y > 0)
    {
        float me = heightMap.Sample(samp, tex);
        
        float t = heightMap.Sample(samp, float2(tex.x, tex.y + 1.0 / gridLengthCount));
        float b = heightMap.Sample(samp, float2(tex.x, tex.y - 1.0 / gridLengthCount));
        float l = heightMap.Sample(samp, float2(tex.x - 1.0 / gridWidthCount, tex.y));
        float r = heightMap.Sample(samp, float2(tex.x + 1.0 / gridWidthCount, tex.y));
        
        float3 tanZ = float3(0.0f, (t - b), 1.0f);
        float3 tanX = float3(1.0f, (r - l), 0.0f);
        float3 N;
        N = cross(tanZ, tanX);
        N = normalize(N);
        
        normal = N;
        
    }
    
    float3 color = surfaceColor * ambientColor;
    for (int i = 0; i < 1; ++i)
    {
        float3 lightPosition = lightPositions[i];
        float3 lightVec = normalize(lightPosition - input.worldPos);
        float3 halfVec = normalize(viewVec + lightVec);
        color += lightColor * surfaceColor.xyz * kd * saturate(dot(normal, lightVec)); //diffuse color
        float nh = dot(normal, halfVec);
        nh = saturate(nh);
        nh = pow(nh, m);
        nh *= ks;
        color += lightColor * nh;
    }
       
    
    //return float4(normalize(normal), 1.0);
    return float4(saturate(color), 1.0);
}
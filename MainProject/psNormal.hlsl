#include "normalHeader.hlsli"

static const float3 surfaceColor = float3(1, 0, 0);

static const float3 lightPositions[2] =
{
    float3(-100, 100, -100),
    float3(100, 100, 100)
};

static const float3 ambientColor = float3(0.5f, 0.5f, 0.5f);
static const float3 lightColor = float3(1.0f, 1.0f, 1.0f);
static const float kd = 0.5, ks = 0.2f, m = 10.0f;
static const float mh = 100.0f;


float4 main(VSOutput input) : SV_TARGET
{
    float3 viewVec = normalize(input.viewVec);
    float3 normal = normalize(input.norm);
    float3 color = surfaceColor.rgb * ambientColor;
   
    for (int i = 0; i < 2; ++i)
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
       
    
    //return float4(normal, 1.0);
    return float4(saturate(color), 1.0);
}
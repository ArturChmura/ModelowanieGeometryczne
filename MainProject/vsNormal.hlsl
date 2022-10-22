#include "normalHeader.hlsli"

cbuffer cbWorld : register(b0) //Vertex Shader constant buffer slot 0
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix inversedViewMatrix;
    matrix projectionMatrix;
};

struct VSNormalIn
{
    float3 pos : SV_POSITION;
    float3 normal : NORMAL;
};


VSOutput main(VSNormalIn i)
{
    VSOutput o = (VSOutput) 0;
	
    
    o.worldPos = mul(modelMatrix, float4(i.pos, 1.0f)).xyz;
	
    o.pos = mul(viewMatrix, float4(o.worldPos, 1.0f));
    o.pos = mul(projectionMatrix, o.pos);
    
    o.norm = mul(modelMatrix, float4(i.normal, 0.0f)).xyz;
    o.norm = normalize(o.norm);
    
    float3 camPos = mul(inversedViewMatrix, float4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
    o.viewVec = camPos - o.worldPos;
    
    return o;
}
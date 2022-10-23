#include "normalHeader.hlsli"

texture2D heightMap : register(t0);


cbuffer cbWorld : register(b0) //Vertex Shader constant buffer slot 0
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix inversedViewMatrix;
    matrix projectionMatrix;
    
    int gridWidthCount;
    int gridLengthCount;
    
    float widthSize;
    float lengthSize;
    
};

struct VSNormalIn
{
    float3 pos : SV_POSITION;
    float3 normal : NORMAL;
};


VSOutput main(VSNormalIn i)
{
    VSOutput o = (VSOutput) 0;
	
    float4 position = float4(i.pos, 1.0f);
    float3 normal = i.normal;
    if (i.pos.y > 0)
    {
        int x = (i.pos.x + widthSize / 2) * ((gridWidthCount - 1) / widthSize) + 0.5;
        int y = (i.pos.z + lengthSize / 2) * ((gridLengthCount - 1) / lengthSize) + 0.5;
        
    
        int2 pos_xy = { x,y };
    
        float height = heightMap[pos_xy];
    
        position.y = height;
        
        
      
        
    }
  
    
    o.worldPos = mul(modelMatrix, position).xyz;
	
    o.pos = mul(viewMatrix, float4(o.worldPos, 1.0f));
    o.pos = mul(projectionMatrix, o.pos);
    
    o.norm = mul(modelMatrix, float4(normal, 0.0f)).xyz;
    o.norm = normalize(o.norm);
    
    float3 camPos = mul(inversedViewMatrix, float4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
    o.viewVec = camPos - o.worldPos;
    
    return o;
}
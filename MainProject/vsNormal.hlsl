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
        int x = (i.pos.x + widthSize / 2) * (gridWidthCount / widthSize);
        int y = (i.pos.z + lengthSize / 2) * (gridLengthCount / lengthSize);
    
        int2 pos_xy = { x,y };
    
        float height = heightMap[pos_xy];
    
        position.y = height;
        
        
        int2 pos_x1y = { x+1, y };
        float upHeight = heightMap[pos_x1y];
        int2 pos_xy1 = { x, y + 1 };
        float rightHeight = heightMap[pos_xy1];
        float dz = height - upHeight;
        float dx = height - rightHeight;
        dz /= 1.0 / (gridLengthCount - 1);
        dx /= 1.0 / (gridWidthCount - 1);
        float3 n = float3(dx, 1, dz);
        n = normalize(n);
        
        normal = n;
        
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
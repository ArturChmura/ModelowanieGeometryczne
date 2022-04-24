#include <vsOUtPolynomial.hlsli>


cbuffer transformations : register(b0)
{
    matrix MVP;
}


[maxvertexcount(100)]
void main(
	point VSOut input[1],
	inout LineStream< GSOutput > output
)
{
    float d = input[0].a.w;
    float t = 0.0f;
	for (int i = 0; i < 100; i++)
	{
		GSOutput element;
        float3 x = input[0].a.xyz + t * input[0].b + t * t * input[0].c + t * t * t * input[0].d;
        element.pos = mul(MVP, float4(x,1.0f));
		output.Append(element);
        t += d / 100.0f;
    }
 
    
    output.RestartStrip();
}
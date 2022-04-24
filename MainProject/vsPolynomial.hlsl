#include <vsOUtPolynomial.hlsli>


VSOut main(VSIn i) 
{
    VSOut o;
    o.a = i.a;
    o.b = i.b;
    o.c = i.c;
    o.d = i.d;
    return o;
    //return float4(i.coef, i.dis);
    //return float4(1, 1, 1, 1);
}
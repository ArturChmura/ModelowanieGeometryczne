#include "bezierStructs.hlsli"

GSBezierIn main(VSBezierIn i)
{
    GSBezierIn o;
    o.x = i.x;
    o.y = i.y;
    o.z = i.z;
    o.SIZE = i.SIZE;
	return o;
}
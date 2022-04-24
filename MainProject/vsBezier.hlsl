#include "bezierStructs.hlsli"

GSIn main(VSIn i)
{
    GSIn o;
    o.x = i.x;
    o.y = i.y;
    o.z = i.z;
    o.SIZE = i.SIZE;
	return o;
}
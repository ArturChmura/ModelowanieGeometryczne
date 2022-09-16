#pragma once
#include <d3d11.h>
#include "SimpleMath.h"
class IntersectionPoint
{
public:
	DirectX::SimpleMath::Vector3 position;
	double u;
	double v;
	double s;
	double t;
	bool found;
	bool last;

};
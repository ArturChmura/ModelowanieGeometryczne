#pragma once
#include "ICutter.h"

class SphereCutter : public ICutter
{
public:
	SphereCutter(float radius);
	// Inherited via ICutter
	virtual float GetHeight(float x, float y) override;

};
#pragma once
#include "ICutter.h"

class FlatCutter : public ICutter
{
public:
	FlatCutter(float radius);
	// Inherited via ICutter
	virtual float GetHeight(float x, float y) override;

};
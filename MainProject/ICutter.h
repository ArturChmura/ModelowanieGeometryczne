#pragma once

class ICutter
{
public:
	ICutter(float radius);
	float GetRadius();
	virtual float GetHeight(float x, float y) = 0;

protected:
	float radius;

};
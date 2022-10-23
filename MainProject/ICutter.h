#pragma once

class ICutter
{
public:
	ICutter(float radius);
	float GetRadius();
	void SetRadius(float radius);
	virtual float GetHeight(float x, float y) = 0;

protected:
	float radius;

};
#pragma once
#include "Camera.h"
#include "SimpleMath.h"
#include "CutterModel.h"

class ICutter
{
public:
	ICutter(float radius);
	float GetRadius();
	void SetRadius(float radius);
	void SetPosition(DirectX::SimpleMath::Vector3 position);
	void Draw(std::shared_ptr<Camera> camera);
	virtual float GetHeight(float x, float y) = 0;

protected:
	float radius;
	DirectX::SimpleMath::Vector3 position;
	std::shared_ptr<CutterModel> model;

};
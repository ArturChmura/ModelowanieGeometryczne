#pragma once
#include "Camera.h"
#include "SimpleMath.h"
#include "CutterModel.h"

class ICutter
{
public:
	ICutter(float radius, float cuttingPartHeight);
	float GetRadius();
	void SetRadius(float radius);
	void SetCuttingPartHeight(float cuttingPartHeight);
	void SetPosition(DirectX::SimpleMath::Vector3 position);
	void Draw(std::shared_ptr<Camera> camera);
	virtual float GetHeight(float x, float y) = 0;
	float GetCuttingPartHeight();
	void GetCuttingPartHeight(float cuttingPartHeight);
	virtual DirectX::SimpleMath::Vector3 GetCuttingPointInDirection(DirectX::SimpleMath::Vector2 radiusPoint, DirectX::SimpleMath::Vector3 direction) = 0;
	virtual bool IsError(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, float maxHeight, std::string& outErrorMessage);
protected:
	float radius;
	float cuttingPartHeight;
	DirectX::SimpleMath::Vector3 position;
	std::shared_ptr<CutterModel> model;

};
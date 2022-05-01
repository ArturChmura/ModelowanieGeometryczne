#pragma once
#include "ICameraMovement.h"

class ArcCameraModel: public ICameraMovement
{
public:
	ArcCameraModel(DirectX::SimpleMath::Vector3 targetPosition, float distance);
		
	DirectX::SimpleMath::Matrix GetViewMatrix() override;

	void Rotate(float firstAxis, float secondAxis);
	void ChangeDistance(float distanceChange);
	void LookAt(float x, float y, float z);

private:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 targetPosition;
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Vector3 upVector = { 0,1,0 };
	void UpdateViewMatrix();
	DirectX::SimpleMath::Vector3 GetCameraDirection();

	// Inherited via ICameraMovement
	virtual void DrawGUI() override;

};
#pragma once
#include "Camera.h"
#include "ICameraMovement.h"
class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(std::shared_ptr<ICameraMovement> cameraMovement, float viewWidth, float viewHeight, float nearZ = 0.1f, float farZ = 100.0f, std::string name = "Orthographic Camera");

	

	virtual DirectX::SimpleMath::Matrix GetPerspectiveMatrix() override;
	virtual DirectX::SimpleMath::Matrix GetViewMatrix() override;

protected:
	float viewWidth;
	float viewHeight;

	DirectX::SimpleMath::Matrix perspectiveMatrix;

	// Inherited via Camera

	virtual void DrawGUI() override;

	// Inherited via Camera
	virtual void RenderScene(std::shared_ptr<IScene> scene) override;

};
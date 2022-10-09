#pragma once
#include "Camera.h"
#include "ICameraMovement.h"
class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera( std::shared_ptr<ICameraMovement> cameraMovement,float fieldOfView, float aspectRatio, float nearZ, float farZ, std::string name = "Perspective Camera");

	void SetFieldOfView(float fov);
	float GetFieldOfView() { return fieldOfView; }

	void SetNearZ(float nearZ);
	float GetNearZ() { return nearZ; }

	void SetFarZ(float farZ);
	float GetFarZ() { return farZ; }

	void SetAspectRatio(float aspectRatio);
	float GetAspectRatio() { return aspectRatio; }

	virtual DirectX::SimpleMath::Matrix GetPerspectiveMatrix() override;

protected:
	float fieldOfView;
	float aspectRatio;
	float nearZ;
	float farZ;
	virtual void UpdatePerspectiveMatrix();

	DirectX::SimpleMath::Matrix perspectiveMatrix;

	// Inherited via Camera
	virtual DirectX::SimpleMath::Matrix GetViewMatrix() override;

	virtual void DrawGUI() override;

	// Inherited via Camera
	virtual void RenderScene(std::shared_ptr<IScene> scene) override;

};
#pragma once
#include "Camera.h"
class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float fieldOfView, float aspectRatio, float nearZ, float farZ);
	void SetFieldOfView(float fov);
	void SetNearZ(float nearZ);
	void SetFarZ(float farZ);
	void SetAspectRatio(float aspectRatio);
	float GetFieldOfView() { return fieldOfView; }
	float GetAspectRatio() { return aspectRatio; }
	float GetNearZ( ) { return nearZ; }
	float GetFarZ( ) { return farZ; }
protected:
	float fieldOfView;
	float aspectRatio;
	float nearZ;
	float farZ;
	void UpdatePerspectiveMatrix();
};
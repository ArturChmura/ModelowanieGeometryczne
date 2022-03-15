#pragma once
#include <DirectXMath.h>
#include "PerspectiveCamera.h"
#include <memory>

class PerspectiveCameraOptionsWindow
{
public:
	PerspectiveCameraOptionsWindow(std::shared_ptr<PerspectiveCamera> perspectiveCamera);
	void Render();
	std::shared_ptr<PerspectiveCamera> perspectiveCamera;
	
private:
	float fieldOfView;
	float aspectRatio;
	float nearZ;
	float farZ;
};
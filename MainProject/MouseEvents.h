#pragma once
#include "Helpers.h"
#include "ArcCameraModel.h"
#include <memory>
class MouseEvents
{
public:
	MouseEvents(std::shared_ptr<ArcCameraModel> camera);
	void HandleMouse();

private:
	float rotationSpeedPerPixel = 0.01f;
	std::shared_ptr<ArcCameraModel> camera;
};
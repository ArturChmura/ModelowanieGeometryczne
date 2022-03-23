#pragma once
#include "Helpers.h"
#include "ArcCameraModel.h"
#include <memory>
#include "Scene.h"
class MouseEvents
{
public:
	MouseEvents(std::shared_ptr<ArcCameraModel> camera, std::shared_ptr<Scene> scene);
	void HandleMouse();

private:
	float rotationSpeedPerPixel = 0.01f;
	std::shared_ptr<ArcCameraModel> camera; 
	std::shared_ptr<Scene> scene;
};
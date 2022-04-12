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
	float cameraRotationSpeedPerPixel = 0.01f;
	float cameraMoveSpeedPerPixel = 1.0f;
	float objectRotationSpeedPerPixel = 0.01f;
	float objectScaleSpeedPerPixel = 0.01f;
	float objectTranslateSpeedPerPixel = 0.05f;
	std::shared_ptr<ArcCameraModel> camera; 
	std::shared_ptr<Scene> scene;
};
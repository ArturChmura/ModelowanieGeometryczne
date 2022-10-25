#pragma once
#include "ArcCameraModel.h"
#include <memory>
#include "Scene.h"

class KeyboardHandler
{
public:
	KeyboardHandler(std::shared_ptr<ArcCameraModel> camera, std::shared_ptr<Scene> scene);
	void HandleKeyboard();

private:
	std::shared_ptr<Scene> scene;
	std::shared_ptr<ArcCameraModel> camera;
};
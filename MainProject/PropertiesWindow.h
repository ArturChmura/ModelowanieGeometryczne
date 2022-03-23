#pragma once
#include <memory>
#include "Scene.h"

class PropertiesWindow
{
public:
	PropertiesWindow(std::shared_ptr<Scene> scene);
	void Render();
	std::shared_ptr<Scene> scene;
};
#pragma once
#include <memory>
#include "Scene.h"

class ObjectsListWindow
{
public:
	ObjectsListWindow(std::shared_ptr<Scene> scene);
	void Render();
	std::shared_ptr<Scene> scene;
};
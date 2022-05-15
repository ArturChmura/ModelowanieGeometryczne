#pragma once
#include <memory>
#include "Scene.h"
#include "IGuiWindow.h"

class ObjectsListWindow : public IGuiWindow
{
public:
	ObjectsListWindow(std::shared_ptr<Scene> scene);
	void Render();
	std::shared_ptr<Scene> scene;
};
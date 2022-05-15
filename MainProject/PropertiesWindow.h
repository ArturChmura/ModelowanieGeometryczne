#pragma once
#include <memory>
#include "Scene.h"
#include "IGuiWindow.h"

class PropertiesWindow : public IGuiWindow
{
public:
	PropertiesWindow(std::shared_ptr<Scene> scene);
	void Render();
	std::shared_ptr<Scene> scene;
};
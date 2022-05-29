#pragma once
#include "Scene.h"
#include <memory>
#include "IGuiWindow.h"

class StartWindow : public IGuiWindow
{
public:
	StartWindow(std::shared_ptr<Scene> scene);
	void Render();
	std::shared_ptr<Scene> scene;
private:
};
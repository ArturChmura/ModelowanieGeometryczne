#pragma once

#include "Scene.h"
#include <memory>
#include "IGuiWindow.h"

class ModelSceneStartWindow : public IGuiWindow
{
public:
	ModelSceneStartWindow(std::shared_ptr<Scene> scene);
	void Render();
	std::shared_ptr<Scene> scene;
private:
};
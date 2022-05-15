#pragma once
#include "Scene.h"
#include <memory>
#include "IGuiWindow.h"

class CameraOptionsWindow : public IGuiWindow
{
public:
	CameraOptionsWindow(std::shared_ptr<Scene> scene);
	void Render();
	std::shared_ptr<Scene> scene;
private:
};
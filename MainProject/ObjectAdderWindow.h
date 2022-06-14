#pragma once
#include <memory>
#include "Scene.h"
#include "BezierSurfaceAdderWindow.h"
#include "IGuiWindow.h"
#include "GregoryAdderWindow.h"

class ObjectAdderWindow : public IGuiWindow
{
public:
	ObjectAdderWindow(std::shared_ptr<Scene> scene, bool* renderGui);
	void Render();
	std::shared_ptr<Scene> scene;
	std::shared_ptr<BezierSurfaceAdderWindow> adder;
	std::shared_ptr<GregoryAdderWindow> gregoryAdder;
	bool surfaceAdderOpen = false;
	bool gregoryAdderOpen = false;
	bool* renderGui;
};
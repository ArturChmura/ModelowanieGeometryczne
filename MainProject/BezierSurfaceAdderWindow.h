#pragma once
#include <memory>
#include "Scene.h"
#include "IGuiWindow.h"

class BezierSurfaceAdderWindow : public IGuiWindow
{
public:
	BezierSurfaceAdderWindow(std::shared_ptr<Scene> scene, bool* open);
	~BezierSurfaceAdderWindow();
	void Render();
	void SetPreview();
	std::shared_ptr<Scene> scene; 
	bool* open;

	int horizontalSlicesCount = 3;
	int verticalSlicesCount = 3;

	float width = 10.0f;
	float height = 10.0f;

	bool cylinder = false;
};
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
	virtual void SetPreview();
	virtual std::shared_ptr<IBezierSurface> AddModel();
	virtual std::tuple<std::shared_ptr<IBezierSurface>, std::vector<std::vector<std::shared_ptr<Point>>>> GetModel() = 0;
	std::shared_ptr<Scene> scene; 
	bool* open;

	int horizontalSlicesCount = 3;
	int verticalSlicesCount = 3;

	float width = 10.0f;
	float height = 10.0f;

	bool cylinder = false;

};

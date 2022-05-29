#pragma once
#include "BezierSurfaceAdderWindow.h"

class BezierSurfaceC2AdderWindow : public BezierSurfaceAdderWindow
{
public:
	BezierSurfaceC2AdderWindow(std::shared_ptr<Scene> scene, bool* open);
	~BezierSurfaceC2AdderWindow();

	// Inherited via BezierSurfaceAdderWindow
	virtual std::tuple<std::shared_ptr<IModel>, std::vector<std::vector<std::shared_ptr<Point>>>> GetModel() override;

};
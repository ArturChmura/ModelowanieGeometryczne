#pragma once
#include "BezierSurfaceAdderWindow.h"

class BezierSurfaceC0AdderWindow : public BezierSurfaceAdderWindow
{
public:
	BezierSurfaceC0AdderWindow(std::shared_ptr<Scene> scene, bool* open);
	~BezierSurfaceC0AdderWindow();

	

	// Inherited via BezierSurfaceAdderWindow
	virtual std::tuple<std::shared_ptr<IModel>, std::vector<std::vector<std::shared_ptr<Point>>>> GetModel() override;


};
#pragma once
#include "BezierSurfaceAdderWindow.h"

class BezierSurfaceC0AdderWindow : public BezierSurfaceAdderWindow
{
public:
	BezierSurfaceC0AdderWindow(std::shared_ptr<Scene> scene, bool* open);
	~BezierSurfaceC0AdderWindow();

	void SetPreview() override;
	void AddModel() override;
};
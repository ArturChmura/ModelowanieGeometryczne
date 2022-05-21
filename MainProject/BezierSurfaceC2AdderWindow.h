#pragma once
#include "BezierSurfaceAdderWindow.h"

class BezierSurfaceC2AdderWindow : public BezierSurfaceAdderWindow
{
public:
	BezierSurfaceC2AdderWindow(std::shared_ptr<Scene> scene, bool* open);
	~BezierSurfaceC2AdderWindow();

	void SetPreview() override;
	void AddModel() override;
};
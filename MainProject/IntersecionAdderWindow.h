#pragma once

#include <memory>
#include "Scene.h"
#include "IGuiWindow.h"
#include "PatchSide.h"
#include "GregoryPatch.h"
#include "IParameterized.h"

class IntersecionAdderWindow : public IGuiWindow
{

public:
	IntersecionAdderWindow(std::shared_ptr<Scene> scene, bool* open, std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2);
	IntersecionAdderWindow(std::shared_ptr<Scene> scene, bool* open, std::shared_ptr<IParameterized> surface);
	~IntersecionAdderWindow();
	void Render();
	std::shared_ptr<IParameterized> surface1;
	std::shared_ptr<IParameterized> surface2;
	std::shared_ptr<Scene> scene;
	bool* open;

	float pointsDistance = 0.1;
	float precision = 0.01;

	bool textNotFound = false;
	bool useCursor = false;
	bool singleSurface;
};

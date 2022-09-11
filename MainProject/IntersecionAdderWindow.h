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
	~IntersecionAdderWindow();
	void Render();
	std::shared_ptr<IParameterized> surface1;
	std::shared_ptr<IParameterized> surface2;
	std::shared_ptr<Scene> scene;
	bool* open;

};

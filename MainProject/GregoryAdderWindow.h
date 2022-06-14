#pragma once
#pragma once
#include <memory>
#include "Scene.h"
#include "IGuiWindow.h"
#include "PatchSide.h"
#include "GregoryPatch.h"

class GregoryAdderWindow : public IGuiWindow
{

public:
	GregoryAdderWindow(std::shared_ptr<Scene> scene, bool* open, std::vector<std::vector<std::shared_ptr<PatchSide>>> cycles);
	~GregoryAdderWindow();
	void Render();
	virtual void SetPreview();
	void AddModel();
	std::vector<std::vector<std::shared_ptr<PatchSide>>> cycles;
	int index = 0;
	std::shared_ptr<Scene> scene;
	bool* open;

	std::shared_ptr<GregoryPatch> GetModel();
};

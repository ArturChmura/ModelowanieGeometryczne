#pragma once
#include <vector>
#include "Camera.h"
#include <memory>
#include "DxVertexModelDrawer.h"

class Scene
{
public:
	void AddCamera(std::shared_ptr<Camera> camera);
	void SetActiveCamera(std::shared_ptr<Camera> camera);
	void AddModel(std::shared_ptr<DxVertexModelDrawer> model)
	{
		models.push_back(model);
	}
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;

	std::vector<std::shared_ptr<DxVertexModelDrawer>> models;
private:
};
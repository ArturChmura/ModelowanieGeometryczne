#pragma once
#include <vector>
#include "Camera.h"
#include <memory>
#include "IModel.h"
#include "Coursor3d.h"

class Scene
{
public:
	Scene(std::shared_ptr<Coursor3d> cursor);
	void AddModel(std::shared_ptr<IModel> model);
	void AddTorus();
	void AddPoint();
	std::vector<std::shared_ptr<IModel>> models;

	std::shared_ptr<Coursor3d> cursor;
	std::shared_ptr<IModel> selectedModel;

	void AddCamera(std::shared_ptr<Camera> camera);
	void SetActiveCamera(std::shared_ptr<Camera> camera);
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;
private:
};
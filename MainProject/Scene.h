#pragma once
#include <vector>
#include "Camera.h"
#include <memory>
#include "IModel.h"
#include "Coursor3d.h"
#include "CompositeModel.h"

class Scene
{
public:
	Scene(std::shared_ptr<Coursor3d> cursor, SIZE windowSize);
	void AddModel(std::shared_ptr<IModel> model);
	void AddTorus();
	void AddPoint();
	void DeleteModel(int modelId);
	void ChangeSelection(int modelId);
	std::vector<std::shared_ptr<IModel>> models;

	std::shared_ptr<Coursor3d> cursor;
	std::shared_ptr<IModel> selectedModel;

	void AddCamera(std::shared_ptr<Camera> camera);
	void SetActiveCamera(std::shared_ptr<Camera> camera);
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;
	std::shared_ptr<CompositeModel> composite; 
	bool IsSelcted(int modelId); 
	void ChangeSelectionFromScreenCoords(float x, float y);
	SIZE windowSize;
private:
};
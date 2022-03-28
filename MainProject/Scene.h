#pragma once
#include <vector>
#include "Camera.h"
#include <memory>
#include "IModel.h"
#include "Coursor3d.h"
#include "CompositeModel.h"
#include "Point.h"
#include "Torus.h"

class Scene
{
public:
	Scene(std::shared_ptr<Coursor3d> cursor, SIZE windowSize);
	std::shared_ptr<Torus> AddTorus();
	std::shared_ptr<Point> AddPoint();
	void AddBezierCurveFromSelectedPoints();
	void DeleteModel(int modelId);
	void ChangeSelection(int modelId);
	std::vector<std::shared_ptr<IModel>> models;
	std::vector<std::shared_ptr<Point>> GetSelectedPoints();
	std::shared_ptr<Coursor3d> cursor;
	std::shared_ptr<IModel> selectedModel; 
	std::vector<std::shared_ptr<Point>> points;

	void AddCamera(std::shared_ptr<Camera> camera);
	void SetActiveCamera(std::shared_ptr<Camera> camera);
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;
	std::shared_ptr<CompositeModel> composite; 
	bool IsSelcted(int modelId); 
	void ChangeSelectionFromScreenCoords(float x, float y);
	SIZE windowSize;
public:
	void AddModel(std::shared_ptr<IModel> model);
private:
};
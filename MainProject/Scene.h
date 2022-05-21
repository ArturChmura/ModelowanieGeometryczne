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
	void AddPoint(std::shared_ptr<Point> point);
	void AddModel(std::shared_ptr<IModel> model);
	void AddBezierCurveC0FromSelectedPoints();
	void AddBezierCurveC2FromSelectedPoints(); 
	void AddBezierCurveInterpolatingFromSelectedPoints();
	void DeleteModel(int modelId);
	void Select(std::shared_ptr<IModel> model);
	void DeselectAll();
	void ChangeSelection(std::shared_ptr<IModel> model);
	std::vector<std::shared_ptr<IModel>> models;
	std::vector<std::shared_ptr<Point>> GetSelectedPoints();
	std::shared_ptr<Coursor3d> cursor;
	std::vector<std::shared_ptr<Point>> points;
	std::shared_ptr<IModel> previewModel;

	void AddCamera(std::shared_ptr<Camera> camera);
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;
	std::shared_ptr<CompositeModel> composite; 
	bool IsSelcted(int modelId); 
	std::shared_ptr<IModel> GetModelFromScreenCoords(float x, float y);
	std::vector<std::shared_ptr<IModel>> GetModelsFromArea(float left, float right, float top, float bottom);
	SIZE windowSize;
	void UpdateCursorPositionFromScreenCoords(DirectX::SimpleMath::Vector2 screenCoords);
	void RemoveSelectedModels();

	void DrawScene();
public:
private:
};
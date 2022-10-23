#pragma once
#include <vector>
#include "Camera.h"
#include <memory>
#include "IModel.h"
#include "Cursor3d.h"
#include "CompositeModel.h"
#include "Point.h"
#include "Torus.h"
#include "BezierSurfaceC0.h"
#include "ConcreteModelSelectorVisitor.h"
#include "IScene.h"

class Scene : public IScene
{
public:
	Scene(std::shared_ptr<Cursor3d> cursor, SIZE windowSize);
	std::shared_ptr<Torus> AddTorus();
	std::shared_ptr<Point> AddPoint();
	void AddModel(std::shared_ptr<IModel> model);
	void AddBezierCurveC0FromSelectedPoints();
	void AddBezierCurveC2FromSelectedPoints(); 
	void AddBezierCurveInterpolatingFromSelectedPoints();
	void RemoveModel(int modelId);
	void Select(std::shared_ptr<IModel> model);
	void DeselectAll();
	void ChangeSelection(std::shared_ptr<IModel> model);
	std::vector<std::shared_ptr<IModel>> models;

	template< typename T>
	std::vector<std::shared_ptr<T>> GetSelectedType();

	std::shared_ptr<Cursor3d> cursor;
	std::shared_ptr<IModel> previewModel;

	std::shared_ptr<CompositeModel> composite; 
	bool IsSelcted(int modelId); 
	std::shared_ptr<IModel> GetModelFromScreenCoords(float x, float y);
	std::vector<std::shared_ptr<IModel>> GetModelsFromArea(float left, float right, float top, float bottom);
	SIZE windowSize;
	void UpdateCursorPositionFromScreenCoords(DirectX::SimpleMath::Vector2 screenCoords);
	void RemoveSelectedModels();
	void Clear();
	void DrawScene() override;
public:
private:
};

template<typename T>
inline std::vector<std::shared_ptr<T>>  Scene::GetSelectedType()
{
	auto selectedModels = composite->GetContainingModels();
	ConcreteModelSelectorVisitor<T> selector;
	auto selectedModelTypes = selector.GetList(selectedModels);
	return selectedModelTypes;
}


#pragma once

#include "IBezierCurve.h"
#include "VirtualPoint.h"

class StraightCurveInterpolating : public IUnmovableModel
{
public:
	StraightCurveInterpolating(std::vector<std::shared_ptr<Point>> points);
	std::vector<std::shared_ptr<Point>> points;
	virtual void UpdateVertices();
	int verticesCount;
	int indicesCount;
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	void AddPoint(std::shared_ptr<Point> point);
	void AddPointToBeggining(std::shared_ptr<Point> point);
	void RemovePoint(int pointId);
	void ResetDrawing();
	void OnRemovedFromScene() override;

	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	std::vector<int> selectedIndexes;
	virtual void RenderGUI() override;


	virtual std::vector<std::shared_ptr<IModel>> GetContainingModels() override;
	MeshInfo meshInfo;

	virtual void Accept(AbstractModelVisitor& visitor) override;
	virtual bool Serializable();
protected:
	bool resetDrawing = true;
	bool isAddingMode = false;
};
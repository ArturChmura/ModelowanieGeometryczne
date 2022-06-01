#pragma once
#include "IModel.h"
#include "Point.h"
#include "PolygonalChain.h"
#include "IUnmovableModel.h"


class IBezierCurve : public IUnmovableModel
{
public:
	IBezierCurve(std::vector<std::shared_ptr<Point>> points, std::string name = "IBezierCurve");
	std::vector<std::shared_ptr<Point>> points;
	virtual std::vector<DirectX::SimpleMath::Vector3> GetBezierPoints() = 0;
	virtual std::vector<DirectX::SimpleMath::Vector3> GetPolygonChainPoints() = 0;

	virtual void UpdateVertices();

	int verticesCount;
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	void AddPoint(std::shared_ptr<Point> point);
	void RemovePoint(int pointId);
	void SetDrawPolygonChain(bool draw = true);
	void ResetDrawing();
	void OnRemovedFromScene() override;

	virtual void ChangeColor(DirectX::SimpleMath::Vector3 color) override;
	std::vector<int> selectedIndexes;
	virtual void RenderGUI() override;


	virtual std::vector<std::shared_ptr<IModel>> GetContainingModels() override;
	MeshInfo meshInfo;
protected:
	bool resetDrawing = true;
	bool drawPolygonChain = false;
	bool isAddingMode = false;
};
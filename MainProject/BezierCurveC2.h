#pragma once

#include "IBezierCurve.h"
#include "Point.h"
#include "ShaderInfoSingleColorVs.h"

class BezierCurveC2 : public IBezierCurve
{
public:
	BezierCurveC2(std::vector<std::shared_ptr<Point>> points);
	std::vector<std::shared_ptr<Point>> bezierPoints;
	// Inherited via IModel

	virtual void Draw(std::shared_ptr<Camera> camera) override;
	void SetRepresentation(bool bezier);
	virtual void RenderGUI() override;

	int indicesCount;
	int verticesCount;
	virtual void UpdateVertices() override;
	void UpdateBezierPoints();

	void OnBezierPointChange(int pointIndex, DirectX::SimpleMath::Vector4 translation);

	std::vector<int> selectedIndexes;
	bool isAddingMode = false;
	void UpdateSlicesCount(std::shared_ptr<Camera> camera);

	std::vector<int> currentSlices;
	std::vector<int> desiredSlices;
	bool bezierRepresentation = false;
	virtual std::shared_ptr<IModel> SelectFromScreenCoords(float x, float y, DirectX::SimpleMath::Matrix VP) override;
private:

};
#pragma once

#include "IBezierCurve.h"
#include "Point.h"
#include "VirtualPoint.h"

class BezierCurveC2 : public IBezierCurve
{
public:
	BezierCurveC2(std::vector<std::shared_ptr<Point>> points);
	std::vector<std::shared_ptr<VirtualPoint>> bezierPoints;
	// Inherited via IModel

	virtual void Draw(std::shared_ptr<Camera> camera) override;
	void SetRepresentation(bool bezier);
	virtual void RenderGUI() override;

	void UpdateBezierPoints();

	void OnBezierPointChange(int pointIndex, DirectX::SimpleMath::Vector4 translation);

	bool bezierRepresentation = false;
	virtual std::shared_ptr<IModel> SelectFromScreenCoords(float x, float y, DirectX::SimpleMath::Matrix VP) override;

	// Inherited via IBezierCurve
	virtual std::vector<DirectX::SimpleMath::Vector3> GetBezierPoints() override;

private:





	// Inherited via IBezierCurve
	virtual std::vector<DirectX::SimpleMath::Vector3> GetPolygonChainPoints() override;



	// Inherited via IBezierCurve
	virtual void Accept(AbstractModelVisitor& visitor) override;


};
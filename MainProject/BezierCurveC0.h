#pragma once

#include "IBezierCurve.h"
#include "Point.h"

class BezierCurveC0 : public IBezierCurve
{
public:
	BezierCurveC0(std::vector<std::shared_ptr<Point>> points);

	// Inherited via IModel
	
	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void RenderGUI() override;
 
	// Inherited via IBezierCurve
	virtual std::vector<DirectX::SimpleMath::Vector3> GetBezierPoints() override;

private:



};
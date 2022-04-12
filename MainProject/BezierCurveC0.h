#pragma once

#include "IBezierCurve.h"
#include "Point.h"
#include "ShaderInfoSingleColorVs.h"

class BezierCurveC0 : public IBezierCurve
{
public:
	BezierCurveC0(std::vector<std::shared_ptr<Point>> points);

	// Inherited via IModel
	
	virtual void Draw(std::shared_ptr<Camera> camera) override;
	virtual void RenderGUI() override;
 
	int indicesCount;
	int verticesCount;
	virtual void UpdateVertices() override;
	void UpdateSlicesCount(std::shared_ptr<Camera> camera);

	std::vector<int> currentSlices;
	std::vector<int> desiredSlices; 

private:

};
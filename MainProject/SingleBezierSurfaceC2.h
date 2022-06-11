#pragma once
#include "ISingleBezierSurface.h"
#include "Point.h"
#include <array>
#include "PolygonalChain.h"

class SingleBezierSurfaceC2 : public ISingleBezierSurface
{
public:
	SingleBezierSurfaceC2(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices);


public:
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	// Inherited via IModel
	virtual void Accept(AbstractModelVisitor& visitor) override;


};
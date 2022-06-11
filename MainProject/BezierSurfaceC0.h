#pragma once

#include "IModel.h"
#include "Point.h"
#include "SingleBezierSurfaceC0.h"
#include "PolygonalChain.h"
#include "IBezierSurface.h"

class BezierSurfaceC0 : public IBezierSurface
{
public:

	BezierSurfaceC0(std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces, int horizontalCount, int verticalCount, std::string name = "Bezier Surface C0");
	std::vector<std::shared_ptr<SingleBezierSurfaceC0>> singleSurfaces;
	virtual void Accept(AbstractModelVisitor& visitor) override;


	// Inherited via IBezierSurface
	virtual std::vector<std::shared_ptr<ISingleBezierSurface>> GetSingleSurfaces() override;


};
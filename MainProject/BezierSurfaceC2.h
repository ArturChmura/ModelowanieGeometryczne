#pragma once

#include "IModel.h"
#include "Point.h"
#include "PolygonalChain.h"
#include "SingleBezierSurfaceC2.h"
#include "IBezierSurface.h"


class BezierSurfaceC2 : public IBezierSurface
{
public:
	BezierSurfaceC2(
		std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces,
		int horizontalCount, int verticalCount, bool isUWrapped, bool isVWrapped, std::string name = "Bezier Surface C2");
	std::vector<std::shared_ptr<SingleBezierSurfaceC2>> singleSurfaces;
	virtual void Accept(AbstractModelVisitor& visitor) override;

	// Inherited via IBezierSurface
	virtual std::vector<std::shared_ptr<ISingleBezierSurface>> GetSingleSurfaces() override;

};
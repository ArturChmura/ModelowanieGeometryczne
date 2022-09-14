#pragma once
#include "ISingleBezierSurface.h"
#include "Point.h"
#include <array>
#include "PolygonalChain.h"

class SingleBezierSurfaceC2 : public ISingleBezierSurface
{
public:
	SingleBezierSurfaceC2(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices,
		int rowIndex, int columnIndex,
		int rowCount, int columnCount);


public:
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	// Inherited via IModel
	virtual void Accept(AbstractModelVisitor& visitor) override;



	// Inherited via ISingleBezierSurface
	virtual DirectX::SimpleMath::Vector3 GetValue(double u, double v) override;


	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(double u, double v) override;


	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(double u, double v) override;


	virtual bool IsUWrapped() override;


	virtual bool IsVWrapped() override;


};
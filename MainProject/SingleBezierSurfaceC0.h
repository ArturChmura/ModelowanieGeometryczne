#pragma once
#include "ISingleBezierSurface.h"
#include "Point.h"
#include <array>
#include "PolygonalChain.h"

class SingleBezierSurfaceC0 : public ISingleBezierSurface
{
public:
	SingleBezierSurfaceC0(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices);


public:
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	// Inherited via IModel
	virtual void Accept(AbstractModelVisitor& visitor) override;


	// Inherited via ISingleBezierSurface
	virtual DirectX::SimpleMath::Vector3 GetValue(float u, float v) override;


	virtual DirectX::SimpleMath::Vector3 GetUDerivativeValue(float u, float v) override;


	virtual DirectX::SimpleMath::Vector3 GetVDerivativeValue(float u, float v) override;


	virtual bool IsUWrapped() override;


	virtual bool IsVWrapped() override;


};
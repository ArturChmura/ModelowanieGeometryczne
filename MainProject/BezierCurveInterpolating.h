#pragma once

#include "IBezierCurve.h"
#include <array>
#include "ShaderInfoPolynomial.h"

class BezierCurveInterpolating : public IBezierCurve
{
public:
	BezierCurveInterpolating(std::vector<std::shared_ptr<Point>> points);
	void CalculatePolynomials();

	// Inherited via IBezierCurve
	virtual void Draw(std::shared_ptr<Camera> camera) override;

	virtual void UpdateVertices() override;

	std::vector<std::array<DirectX::SimpleMath::Vector3, 4>> coef;


	std::shared_ptr<ShaderInfoPolynomial> shaderInfoPolynomial;
	int indicesCount = 0;
	int verticesCount = 0;

};
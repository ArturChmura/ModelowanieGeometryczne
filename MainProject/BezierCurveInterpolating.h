#pragma once

#include "IBezierCurve.h"
#include <array>
#include "VirtualPoint.h"

class BezierCurveInterpolating : public IBezierCurve
{
public:
	BezierCurveInterpolating(std::vector<std::shared_ptr<Point>> points);
	void CalculatePolynomials();

	// Inherited via IBezierCurve
	virtual void Draw(std::shared_ptr<Camera> camera) override;


	std::vector<std::shared_ptr<VirtualPoint>> bezierPoints;
	// Inherited via IBezierCurve
	virtual std::vector<DirectX::SimpleMath::Vector3> GetBezierPoints() override;


	void SetRepresentation(bool bezier);
	virtual void RenderGUI() override;
	bool bezierRepresentation = false;
};


struct Coef
{
	DirectX::XMFLOAT4 A;
	DirectX::XMFLOAT3 B;
	DirectX::XMFLOAT3 C;
	DirectX::XMFLOAT3 D;
};
#pragma once
#include <memory>
class IModel;

class Point;
class Torus;
class BezierCurveC0;
class BezierCurveC2;
class BezierCurveInterpolating;
class SingleBezierSurfaceC0;
class BezierSurfaceC0;
class SingleBezierSurfaceC2;
class BezierPatchC2GPU;
class BezierSurfaceC2;
class CompositeModel;

class AbstractModelVisitor
{
public:
	virtual void Accept(std::shared_ptr<Point> point) = 0;
	virtual void Accept(std::shared_ptr<Torus> torus) = 0;
	virtual void Accept(std::shared_ptr<BezierCurveC0> curve) = 0;
	virtual void Accept(std::shared_ptr<BezierCurveC2> curve) = 0;
	virtual void Accept(std::shared_ptr<BezierCurveInterpolating> curve) = 0;
	virtual void Accept(std::shared_ptr<SingleBezierSurfaceC0> patch) = 0;
	virtual void Accept(std::shared_ptr<BezierSurfaceC0> surface) = 0;
	virtual void Accept(std::shared_ptr<SingleBezierSurfaceC2> patch) = 0;
	virtual void Accept(std::shared_ptr<BezierSurfaceC2> surface) = 0;
	virtual void Accept(std::shared_ptr<CompositeModel> surface) = 0;
};
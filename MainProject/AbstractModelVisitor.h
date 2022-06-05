#pragma once

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
	void Visit(IModel& model);

	virtual void Accept(Point& point) = 0;
	virtual void Accept(Torus& torus) = 0;
	virtual void Accept(BezierCurveC0& curve) = 0;
	virtual void Accept(BezierCurveC2& curve) = 0;
	virtual void Accept(BezierCurveInterpolating& curve) = 0;
	virtual void Accept(SingleBezierSurfaceC0& patch) = 0;
	virtual void Accept(BezierSurfaceC0& surface) = 0;
	virtual void Accept(SingleBezierSurfaceC2& patch) = 0;
	virtual void Accept(BezierSurfaceC2& surface) = 0;
	virtual void Accept(CompositeModel& surface) = 0;
};
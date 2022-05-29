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

class SerializationVisitor
{
public:
	void Visit(IModel& model);

	void Accept(Point& point);
	void Accept(Torus& torus) ;
	void Accept(BezierCurveC0& curve) ;
	void Accept(BezierCurveC2& curve) ;
	void Accept(BezierCurveInterpolating& curve) ;
	void Accept(SingleBezierSurfaceC0& patch) ;
	void Accept(BezierSurfaceC0& surface) ;
	void Accept(SingleBezierSurfaceC2& patch) ;
	void Accept(BezierSurfaceC2& surface) ;
	void Accept(CompositeModel& surface);
};
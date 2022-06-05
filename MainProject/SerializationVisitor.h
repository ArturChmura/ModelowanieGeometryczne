#pragma once
#include "AbstractModelVisitor.h"

class SerializationVisitor : public AbstractModelVisitor
{
public:

	void Accept(std::shared_ptr<Point> point);
	void Accept(std::shared_ptr<Torus> torus) ;
	void Accept(std::shared_ptr<BezierCurveC0> curve) ;
	void Accept(std::shared_ptr<BezierCurveC2> curve) ;
	void Accept(std::shared_ptr<BezierCurveInterpolating> curve) ;
	void Accept(std::shared_ptr<SingleBezierSurfaceC0> patch) ;
	void Accept(std::shared_ptr<BezierSurfaceC0> surface) ;
	void Accept(std::shared_ptr<SingleBezierSurfaceC2> patch) ;
	void Accept(std::shared_ptr<BezierSurfaceC2> surface) ;
	void Accept(std::shared_ptr<CompositeModel> surface);
};
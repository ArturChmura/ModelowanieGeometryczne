#pragma once
#include "AbstractModelVisitor.h"
class EmptyModelVisitor : public AbstractModelVisitor
{
	// Inherited via AbstractModelVisitor
	virtual void Accept(std::shared_ptr<Point> point) override;
	virtual void Accept(std::shared_ptr<Torus> torus) override;
	virtual void Accept(std::shared_ptr<BezierCurveC0> curve) override;
	virtual void Accept(std::shared_ptr<BezierCurveC2> curve) override;
	virtual void Accept(std::shared_ptr<BezierCurveInterpolating> curve) override;
	virtual void Accept(std::shared_ptr<SingleBezierSurfaceC0> patch) override;
	virtual void Accept(std::shared_ptr<BezierSurfaceC0> surface) override;
	virtual void Accept(std::shared_ptr<SingleBezierSurfaceC2> patch) override;
	virtual void Accept(std::shared_ptr<BezierSurfaceC2> surface) override;
	virtual void Accept(std::shared_ptr<CompositeModel> surface) override;
	virtual void Accept(std::shared_ptr<GregoryPatch> surface) override;
	virtual void Accept(std::shared_ptr<SingleGregoryPatch> surface) override;



	// Inherited via AbstractModelVisitor
	virtual void Accept(std::shared_ptr<StraightCurveInterpolating> curve) override;


};
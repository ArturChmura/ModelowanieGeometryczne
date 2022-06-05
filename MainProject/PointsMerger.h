#pragma once
#include <vector>
#include "AbstractModelVisitor.h"
#include "Scene.h"
#include <unordered_set>

class IBezierCurve;

class PointsMerger : AbstractModelVisitor
{
public:
	void MergePoints(std::shared_ptr<Scene> scene);

	// Inherited via AbstractModelVisitor
	virtual void Accept(Point& point) override;

	virtual void Accept(Torus& torus) override;


	virtual void Accept(BezierCurveC0& curve) override;


	virtual void Accept(BezierCurveC2& curve) override;


	virtual void Accept(BezierCurveInterpolating& curve) override;

	void AcceptBezierCurve(IBezierCurve& curve);


	virtual void Accept(SingleBezierSurfaceC0& patch) override;


	virtual void Accept(BezierSurfaceC0& surface) override;


	virtual void Accept(SingleBezierSurfaceC2& patch) override;


	virtual void Accept(BezierSurfaceC2& surface) override;


	virtual void Accept(CompositeModel& surface) override;
private:
	std::unordered_set<int> pointsIds;
	std::shared_ptr<Point> mergedPoint;

};
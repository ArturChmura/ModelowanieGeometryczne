#pragma once
#include <vector>
#include "AbstractModelVisitor.h"
#include "Scene.h"
#include <unordered_set>

class IBezierCurve;

class PointsMerger : AbstractModelVisitor
{
public:
	void MergePoints(std::shared_ptr<Scene> scene, std::vector<std::shared_ptr<Point>> points);

	// Inherited via AbstractModelVisitor
	virtual void Accept(std::shared_ptr<Point> point) override;
	virtual void Accept(std::shared_ptr<Torus> torus) override;
	virtual void Accept(std::shared_ptr<BezierCurveC0> curve) override;
	virtual void Accept(std::shared_ptr<BezierCurveC2> curve) override;
	virtual void Accept(std::shared_ptr<BezierCurveInterpolating> curve) override;
	void AcceptBezierCurve(std::shared_ptr < IBezierCurve> curve);
	virtual void Accept(std::shared_ptr<SingleBezierSurfaceC0> patch) override;
	virtual void Accept(std::shared_ptr<BezierSurfaceC0> surface) override;
	virtual void Accept(std::shared_ptr<SingleBezierSurfaceC2> patch) override;
	virtual void Accept(std::shared_ptr<BezierSurfaceC2> surface) override;
	virtual void Accept(std::shared_ptr<CompositeModel> surface) override;
	virtual void Accept(std::shared_ptr<GregoryPatch> surface) override;
	virtual void Accept(std::shared_ptr<SingleGregoryPatch> surface) override;
private:
	std::unordered_set<int> pointsIds;
	std::shared_ptr<Point> mergedPoint;





	// Inherited via AbstractModelVisitor
	virtual void Accept(std::shared_ptr<StraightCurveInterpolating> curve) override;


};
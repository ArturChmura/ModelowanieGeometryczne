#pragma once
#include "IParameterized.h"
#include <Eigen/Dense>
#include "SimpleMath.h"
#include "IntersectionPoint.h"
#include "IModel.h"
#include "Pair.h"

class IntersectionFinder
{
public:
	IntersectionFinder(double newtonStartDistance, double newtownMinNorm, bool useCursor, DirectX::SimpleMath::Vector3 cursorPosition);

	std::shared_ptr<std::vector<IntersectionPoint>> FindIntersection(
		std::shared_ptr<IParameterized> surface1,
		std::shared_ptr <IParameterized> surface2
	);	
	
	std::shared_ptr<std::vector<IntersectionPoint>> FindSelfIntersection(
		std::shared_ptr<IParameterized> surface
	);
	double minDistanceSelfDivision = 0.2;
	IntersectionPoint FindStartPointSelf(
		std::shared_ptr<IParameterized> surface
	);

	std::shared_ptr<std::vector<IntersectionPoint>> FindAllPoints(std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2, IntersectionPoint P0);

	void FindNextPointsInDirection(std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2, IntersectionPoint P0, DirectX::SimpleMath::Vector3 tangent,bool flip, bool& loopFound, std::vector<IntersectionPoint>& direction1Positions);

	IntersectionPoint FindNearestPoint(
		std::shared_ptr<IParameterized> surface1, 
		std::shared_ptr <IParameterized> surface2,
		double uStart = 0.5,
		double vStart = 0.5,
		double sStart = 0.5,
		double tStart = 0.5,
		float u1 = 0,
		float u2 = 1,
		float v1 = 0,
		float v2 = 1,
		float s1 = 0,
		float s2 = 1,
		float t1 = 0,
		float t2 = 1
	);

	Pair<double> FindStartingPointFromPosition(std::shared_ptr<IParameterized> surface, DirectX::SimpleMath::Vector3 position);

	IntersectionPoint FindNextPoint(
		bool flip,
		std::shared_ptr<IParameterized> surface1,
		std::shared_ptr <IParameterized> surface2,
		IntersectionPoint P0,
		double u1 = 0,
		double u2 = 1,
		double v1 = 0,
		double v2 = 1,
		double s1 = 0,
		double s2 = 1,
		double t1 = 0,
		double t2 = 1
	);

	void PrintMatrix(Eigen::MatrixXd m);

	void PrintVector(DirectX::SimpleMath::Vector3 v);

	void SetCursorPosition(float x, float y, float z);

private:
	void UpdateBest(double u, double v, double& bestU, double& bestV, double& bestDistance, std::shared_ptr<IParameterized> surface, DirectX::SimpleMath::Vector3 position);
	bool useCursor;
	DirectX::SimpleMath::Vector3 cursorPosition;
	double newtonStartDistance;
	double newtownMinNorm;
	int maxTriesFirstPoints = 100;
	int surfaceSplitsOnCursorStart = 20;
};
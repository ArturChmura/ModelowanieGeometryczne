#pragma once
#include "IParameterized.h"
#include <Eigen/Dense>
#include "SimpleMath.h"
#include "IntersectionPoint.h"
#include "IModel.h"

class IntersectionFinder
{
public:
	std::shared_ptr<IModel> FindIntersection(
		std::shared_ptr<IParameterized> surface1,
		std::shared_ptr <IParameterized> surface2
	);

	void FindNextPointsInDirection(std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2, IntersectionPoint P0, DirectX::SimpleMath::Vector3 tangent,bool flip, bool& loopFound, std::vector<DirectX::SimpleMath::Vector3>& direction1Positions);

	IntersectionPoint FindNearestPoint(
		std::shared_ptr<IParameterized> surface1, 
		std::shared_ptr <IParameterized> surface2,
		float u1 = 0,
		float u2 = 1,
		float v1 = 0,
		float v2 = 1,
		float s1 = 0,
		float s2 = 1,
		float t1 = 0,
		float t2 = 1
	);

	IntersectionPoint FindNextPoint(
		bool flip,
		std::shared_ptr<IParameterized> surface1,
		std::shared_ptr <IParameterized> surface2,
		IntersectionPoint P0,
		float u1 = 0,
		float u2 = 1,
		float v1 = 0,
		float v2 = 1,
		float s1 = 0,
		float s2 = 1,
		float t1 = 0,
		float t2 = 1
	);

	void PrintMatrix(Eigen::MatrixXd m);

	void PrintVector(DirectX::SimpleMath::Vector3 v);

	double newtonStartDistance = 0.1;
private:
	float GetInRange(float value, float min, float max);

	double newtownMinNorm = 0.01;
};
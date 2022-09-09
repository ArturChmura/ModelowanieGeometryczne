#pragma once
#include "IParameterized.h"
#include <Eigen/Dense>

class IntersectionFinder
{
public:
	void FindIntersection(
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

	void PrintMatrix(Eigen::MatrixXd m);
};
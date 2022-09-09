#include "IntersectionFinder.h"
#include "SimpleMath.h"
#include <Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string> 

using Eigen::MatrixXd; 
using Eigen::MatrixXf;
using Eigen::Matrix3d;

using namespace DirectX::SimpleMath;

void IntersectionFinder::FindIntersection(
	std::shared_ptr<IParameterized> surface1,
	std::shared_ptr<IParameterized> surface2,
	float u1,
	float u2,
	float v1,
	float v2,
	float s1,
	float s2,
	float t1,
	float t2
)
{
	double u = (u1 + u2) / 2, v = (v1 + v2) / 2, s = (s1 + s2) / 2, t = (t1 + t2) / 2;
	double L1 = 0, L2 = 0, L3 = 0, L4 = 0;
	MatrixXd Hq(7, 8);
	Hq.setZero();

	MatrixXd H(7, 1);
	H.setZero();

	float norm = 9999;
	int i = 0;
	while (norm > 0.0001)
	{
		auto value1 = surface1->GetValue(u, v);
		auto value2 = surface2->GetValue(s, t);

		H(0, 0) = value1.x - value2.x;
		H(1, 0) = value1.y - value2.y;
		H(2, 0) = value1.z - value2.z;

		H(3, 0) = u - ((u1 + u2) / 2 + sin(L1) * (u2 - u1) / 2);
		H(4, 0) = v - ((v1 + v2) / 2 + sin(L2) * (v2 - v1) / 2);
		H(5, 0) = s - ((s1 + s2) / 2 + sin(L3) * (s2 - s1) / 2);
		H(6, 0) = t - ((t1 + t2) / 2 + sin(L4) * (t2 - t1) / 2);
		PrintMatrix(H);
		auto dU = surface1->GetUDerivativeValue(u, v);
		auto dV = surface1->GetVDerivativeValue(u, v);
		auto dS = surface2->GetUDerivativeValue(s, t);
		auto dT = surface2->GetVDerivativeValue(s, t);

		Hq(0, 0) = dU.x;
		Hq(0, 1) = dV.x;
		Hq(0, 2) = -dS.x;
		Hq(0, 3) = -dT.x;

		Hq(1, 0) = dU.y;
		Hq(1, 1) = dV.y;
		Hq(1, 2) = -dS.y;
		Hq(1, 3) = -dT.y;

		Hq(2, 0) = dU.z;
		Hq(2, 1) = dV.z;
		Hq(2, 2) = -dS.z;
		Hq(2, 3) = -dT.z;

		Hq(3, 0) = 1;
		Hq(4, 1) = 1;
		Hq(5, 2) = 1;
		Hq(6, 3) = 1;

		Hq(3, 4) = -(u2 - u1) / 2.0 * cos(L1);
		Hq(4, 5) = -(v2 - v1) / 2.0 * cos(L2);
		Hq(5, 6) = -(s2 - s1) / 2.0 * cos(L3);
		Hq(6, 7) = -(t2 - t1) / 2.0 * cos(L4);
		PrintMatrix(Hq);
		auto HH = Hq * Hq.transpose();

		//PrintMatrix(HH);

		MatrixXd y = HH.colPivHouseholderQr().solve(-H);

		//PrintMatrix(y);

		
		MatrixXd dq = Hq.transpose() * y;

		PrintMatrix(dq);

		u += dq(0, 0);
		v += dq(1, 0);
		s += dq(2, 0);
		t += dq(3, 0);
		L1 += dq(4, 0);
		L2 += dq(5, 0);
		L3 += dq(6, 0);
		L4 += dq(7, 0);

		norm = H.norm();

		i++;
		std::wstringstream wss;
		wss << "\n i = " << i << "\n norm = " << norm << "\n (u,v,s,t) =  " << u << "  " << v << "  " << s << "  " << t << "  ";
		OutputDebugString(wss.str().c_str());
	}
}


void IntersectionFinder::PrintMatrix(
	MatrixXd m
)
{

	OutputDebugString(L" \n");
	for (size_t i = 0; i < m.rows(); i++)
	{

		for (size_t j = 0; j < m.cols(); j++)
		{
			float e = m(i, j);
			std::wstringstream wss;
			wss << std::fixed << std::setprecision(2) << e;
			OutputDebugString(wss.str().c_str());
			OutputDebugString(L" ");
		}
		OutputDebugString(L" \n");
	}
	OutputDebugString(L" \n");
}

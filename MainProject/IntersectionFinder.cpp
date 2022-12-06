#include "IntersectionFinder.h"
#include "SimpleMath.h"
#include <Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string> 
#include "BezierCurveInterpolating.h"
#include "IntersectionCurve.h"
#include "Helpers.h"
using Eigen::MatrixXd;
using Eigen::MatrixXf;
using Eigen::Matrix3d;

using namespace DirectX::SimpleMath;

IntersectionFinder::IntersectionFinder(double newtonStartDistance, double newtownMinNorm, bool useCursor, DirectX::SimpleMath::Vector3 cursorPosition)
{
	this->newtonStartDistance = newtonStartDistance;
	this->newtownMinNorm = newtownMinNorm;
	this->useCursor = useCursor;
	this->cursorPosition = cursorPosition;
}

std::shared_ptr<std::vector<IntersectionPoint>> IntersectionFinder::FindSelfIntersection(std::shared_ptr<IParameterized> surface)
{
	float minU1 = surface->IsUWrapped() ? -1 : 0;
	float minV1 = surface->IsVWrapped() ? -1 : 0;
	float maxU1 = surface->IsUWrapped() ? 2 : 1;
	float maxV1 = surface->IsVWrapped() ? 2 : 1;

	auto closePoint = FindStartPointSelf(surface);


	auto P0 = FindNearestPoint(surface, surface, closePoint.u, closePoint.v, closePoint.s, closePoint.t, minU1, maxU1, minV1, maxV1, minU1, maxU1, minV1, maxV1);


	auto toClose = [=](double u, double v, bool wrapped)
	{
		if (u > v)
		{
			std::swap(u, v);
		}
		auto d = v - u;

		if (wrapped)
		{
			auto dN = u - (v - 1);
			if (dN < d)
			{
				d = dN;
			}
		}
		if (d < minDistanceSelfDivision)
		{
			return true;
		}
		return false;
	};

	if (!P0.found || (toClose(P0.u, P0.s, surface->IsUWrapped()) && toClose(P0.v, P0.t, surface->IsVWrapped())))
	{
		//auto firstHalf = std::make_shared<HalfSurface>(surface, false, 0);
		//auto secondHalf = std::make_shared<HalfSurface>(surface, false, 1);

		//auto P0 = FindNearestPoint(firstHalf, secondHalf, u, v, s, t, minU1, maxU1, minV1, maxV1, minU1, maxU1, minV1, maxV1);
		//auto [uSurface, vSurface] = firstHalf->GetSurfaceUV(P0.u, P0.v);
		//auto [sSurface, tSurface] = secondHalf->GetSurfaceUV(P0.s, P0.t);
		//P0.u = uSurface;
		//P0.v = vSurface;
		//P0.s = sSurface;
		//P0.t = tSurface;
		//if (!P0.found || (toClose(uSurface, sSurface) && toClose(vSurface, tSurface)))
		//{
		return nullptr;
		//}
	}



	return FindAllPoints(surface, surface, P0);
}

IntersectionPoint IntersectionFinder::FindStartPointSelf(std::shared_ptr<IParameterized> surface)
{

	double uBest = 0.0, vBest = 0.0, sBest = 0.0, tBest = 0.0, distanceBest = 99999.9;

	auto toClose = [=](double u, double v, bool wrapped)
	{
		if (u > v)
		{
			std::swap(u, v);
		}
		auto d = v - u;

		if (wrapped)
		{
			auto dN = u - (v - 1);
			if (dN < d)
			{
				d = dN;
			}
		}
		if (d < minDistanceSelfDivision)
		{
			return true;
		}
		return false;
	};

	int patchDivisionsCount = 10;
	int divisionsIterationCount = 4;

	if (useCursor)
	{
		auto uv = FindStartingPointFromPosition(surface, cursorPosition);
		uBest = uv.a;
		vBest = uv.b;
		for (int k = 0; k < patchDivisionsCount; k++)
		{
			for (int l = 0; l < patchDivisionsCount; l++)
			{
				double s = sBest + 1.0/ patchDivisionsCount * (k + 0.5);
				double t = tBest + 1.0 / patchDivisionsCount * (l + 0.5);

				s = GetInRange(s, 0.0, 1.0);
				t = GetInRange(t, 0.0, 1.0);

				if (toClose(uBest, s, surface->IsUWrapped()) && toClose(vBest, t, surface->IsVWrapped()))
				{
					continue;
				}

				auto uvPos = surface->GetValue(uBest, vBest);
				auto stPos = surface->GetValue(s, t);

				auto distance = Vector3::Distance(uvPos, stPos);
				if (distance < distanceBest)
				{
					distanceBest = distance;
					sBest = s;
					tBest = t;
				}
			}
		}

	}

	for (int iter = useCursor ? 1: 0; iter < divisionsIterationCount; iter++)
	{
		double currentPatchWidth = 1.0 / std::pow(patchDivisionsCount, iter);
		double smallPatchWidth = currentPatchWidth / patchDivisionsCount;

		double currentUBest = 0.0;
		double currentVBest = 0.0;
		double currentSBest = 0.0;
		double currentTBest = 0.0;
		double currentDistBest = 99999.0;

		for (int i = 0; i < patchDivisionsCount; i++)
		{
			for (int j = 0; j < patchDivisionsCount; j++)
			{
				for (int k = 0; k < patchDivisionsCount; k++)
				{
					for (int l = 0; l < patchDivisionsCount; l++)
					{
						double u = uBest + smallPatchWidth * (i + 0.5);
						double v = vBest + smallPatchWidth * (j + 0.5);
						double s = sBest + smallPatchWidth * (k + 0.5);
						double t = tBest + smallPatchWidth * (l + 0.5);

						u = GetInRange(u, 0.0, 1.0);
						v = GetInRange(v, 0.0, 1.0);
						s = GetInRange(s, 0.0, 1.0);
						t = GetInRange(t, 0.0, 1.0);

						if (toClose(u , s, surface->IsUWrapped())  && toClose(v, t, surface->IsVWrapped()))
						{
							continue;
						}

						auto uvPos = surface->GetValue(u, v);
						auto stPos = surface->GetValue(s, t);

						auto distance = Vector3::Distance(uvPos, stPos);
						if (distance < currentDistBest)
						{
							currentDistBest = distance;
							currentUBest = u;
							currentVBest = v;
							currentSBest = s;
							currentTBest = t;
						}
					}
				}
			}
		}
		if (currentDistBest < distanceBest)
		{
			distanceBest = currentDistBest;
			uBest = currentUBest;
			vBest = currentVBest;
			sBest = currentSBest;
			tBest = currentTBest;
		}


	}

	IntersectionPoint intersectionPoint;
	intersectionPoint.u = uBest;
	intersectionPoint.v = vBest;
	intersectionPoint.s = sBest;
	intersectionPoint.t = tBest;

	return intersectionPoint;
}



std::shared_ptr<std::vector<IntersectionPoint>> IntersectionFinder::FindIntersection(std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2)
{
	float minU1 = surface1->IsUWrapped() ? -1 : 0;
	float minU2 = surface2->IsUWrapped() ? -1 : 0;
	float minV1 = surface1->IsVWrapped() ? -1 : 0;
	float minV2 = surface2->IsVWrapped() ? -1 : 0;

	float maxU1 = surface1->IsUWrapped() ? 2 : 1;
	float maxU2 = surface2->IsUWrapped() ? 2 : 1;
	float maxV1 = surface1->IsVWrapped() ? 2 : 1;
	float maxV2 = surface2->IsVWrapped() ? 2 : 1;

	Pair<double> uv, st;
	if (useCursor)
	{
		uv = FindStartingPointFromPosition(surface1, cursorPosition);
		st = FindStartingPointFromPosition(surface2, cursorPosition);
		//OutputDebugString(L"Znaleziono punkt");
	}
	else
	{
		uv = { 0.5,0.5 };
		st = { 0.5,0.5 };
	}


	auto P0 = FindNearestPoint(surface1, surface2, uv.a, uv.b, st.a, st.b, minU1, maxU1, minV1, maxV1, minU2, maxU2, minV2, maxV2);
	if (!P0.found)
	{
		return nullptr;
	}

	return FindAllPoints(surface1, surface2, P0);
}

std::shared_ptr<std::vector<IntersectionPoint>> IntersectionFinder::FindAllPoints(std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2, IntersectionPoint P0)
{
	auto dU1 = surface1->GetUDerivativeValue(P0.u, P0.v);
	auto dV1 = surface1->GetVDerivativeValue(P0.u, P0.v);

	auto dU2 = surface2->GetUDerivativeValue(P0.s, P0.t);
	auto dV2 = surface2->GetVDerivativeValue(P0.s, P0.t);

	auto np = dU1.Cross(dV1);
	auto nq = dU2.Cross(dV2);

	auto tangent = np.Cross(nq);
	tangent.Normalize();
	bool loopFound = false;
	std::vector<IntersectionPoint> direction1Positions;
	std::vector<IntersectionPoint> direction2Positions;


	FindNextPointsInDirection(surface1, surface2, P0, tangent, false, loopFound, direction1Positions);
	if (!loopFound)
	{
		tangent = -tangent;
		FindNextPointsInDirection(surface1, surface2, P0, tangent, true, loopFound, direction2Positions);
	}


	std::shared_ptr<std::vector<IntersectionPoint>> positions = std::make_shared< std::vector<IntersectionPoint>>();
	for (int i = direction2Positions.size() - 1; i >= 0; i--)
	{
		positions->push_back(direction2Positions[i]);
	}
	positions->push_back(P0);
	for (int i = 0; i < direction1Positions.size(); i++)
	{
		positions->push_back(direction1Positions[i]);
	}




	return positions;
}

void IntersectionFinder::FindNextPointsInDirection(std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2, IntersectionPoint P0, Vector3 tangent, bool flip, bool& loopFound, std::vector<IntersectionPoint>& direction1Positions)
{
	auto nearest = P0;
	if (flip)
	{
		tangent = -tangent;
	}
	do
	{
		auto prev = nearest;
		nearest = FindNextPoint(flip, surface1, surface2, nearest);
		if (nearest.found)
		{
			auto newDot = (nearest.position - P0.position).Dot(tangent);
			auto prevDot = (prev.position - P0.position).Dot(tangent);

			if (newDot > 0 && prevDot < 0)
			{
				int e = 0;
			}

			if (newDot > 0 && newDot < newtonStartDistance && prevDot < 0 && prevDot > -newtonStartDistance)
			{
				loopFound = true;
				direction1Positions.push_back(nearest);
				break;
			}

			direction1Positions.push_back(nearest);


		}
	} while (nearest.found && !nearest.last);
}

IntersectionPoint IntersectionFinder::FindNearestPoint(
	std::shared_ptr<IParameterized> surface1,
	std::shared_ptr<IParameterized> surface2,
	double uStart,
	double vStart,
	double sStart,
	double tStart,
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
	double u = uStart;
	double v = vStart;
	double s = sStart;
	double t = tStart;
	double L1 = std::asin((u - (u1 + u2) / 2) * 2 / (u1 - u2));
	double L2 = std::asin((v - (v1 + v2) / 2) * 2 / (v1 - v2));
	double L3 = std::asin((s - (s1 + s2) / 2) * 2 / (s1 - s2));
	double L4 = std::asin((t - (t1 + t2) / 2) * 2 / (t1 - t2));


	float norm = 9999;
	int i = 0;
	while (norm > newtownMinNorm)
	{
		auto value1 = surface1->GetValue(u, v);
		auto value2 = surface2->GetValue(s, t);

		auto dU = surface1->GetUDerivativeValue(u, v);
		auto dV = surface1->GetVDerivativeValue(u, v);
		auto dS = surface2->GetUDerivativeValue(s, t);
		auto dT = surface2->GetVDerivativeValue(s, t);

		std::wstringstream wss;
		/*wss << "\n i = " << i
			<< "\n norm = " << norm
			<< "\n (u,v,s,t) =  " << u << "  " << v << "  " << s << "  " << t << "  "
			<< "\n v1.x, v1.y, v1.z =  " << value1.x << "  " << value1.y << "  " << value1.z
			<< "\n v2.x, v2.y, v2.z =  " << value2.x << "  " << value2.y << "  " << value2.z

			<< "\n v1du.x, v1du.y, v1du.z =  " << dU.x << "  " << dU.y << "  " << dU.z
			<< "\n v1dv.x, v1dv.y, v1dv.z =  " << dV.x << "  " << dV.y << "  " << dV.z

			<< "\n v2du.x, v2du.y, v2du.z =  " << dS.x << "  " << dS.y << "  " << dS.z
			<< "\n v2dv.x, v2dv.y, v2dv.z =  " << dT.x << "  " << dT.y << "  " << dT.z
			<< "\n\n";
		OutputDebugString(wss.str().c_str());*/

		MatrixXd H(7, 1);
		H.setZero();
		H(0, 0) = value1.x - value2.x;
		H(1, 0) = value1.y - value2.y;
		H(2, 0) = value1.z - value2.z;

		H(3, 0) = u - ((u1 + u2) / 2 + sin(L1) * (u2 - u1) / 2);
		H(4, 0) = v - ((v1 + v2) / 2 + sin(L2) * (v2 - v1) / 2);
		H(5, 0) = s - ((s1 + s2) / 2 + sin(L3) * (s2 - s1) / 2);
		H(6, 0) = t - ((t1 + t2) / 2 + sin(L4) * (t2 - t1) / 2);
		/*	OutputDebugString(L"H: \n");
			PrintMatrix(H);*/

		MatrixXd Hq(7, 8);
		Hq.setZero();
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

		/*	OutputDebugString(L"Hq: \n");
			PrintMatrix(Hq);*/
		MatrixXd HH = Hq * Hq.transpose();
		/*	OutputDebugString(L"HH: \n");
			PrintMatrix(HH);*/

		MatrixXd y = HH.completeOrthogonalDecomposition().solve(-H);
		/*	OutputDebugString(L"y: \n");
			PrintMatrix(y);*/

		MatrixXd dq = Hq.transpose() * y;
		/*	OutputDebugString(L"dq: \n");
			PrintMatrix(dq);*/


		u += dq(0, 0);
		v += dq(1, 0);
		s += dq(2, 0);
		t += dq(3, 0);
		L1 += dq(4, 0);
		L2 += dq(5, 0);
		L3 += dq(6, 0);
		L4 += dq(7, 0);

		u = GetInRange(u, u1, u2);
		v = GetInRange(v, v1, v2);
		s = GetInRange(s, s1, s2);
		t = GetInRange(t, t1, t2);

		norm = H.norm();

		i++;
		if (i > maxTriesFirstPoints)
		{

			IntersectionPoint intersectionPoint;
			intersectionPoint.found = false;
			return intersectionPoint;
		}
	}
	IntersectionPoint intersectionPoint;
	intersectionPoint.found = true;
	intersectionPoint.position = (surface1->GetValue(u, v) + surface2->GetValue(s, t)) / 2;
	intersectionPoint.u = u;
	intersectionPoint.v = v;
	intersectionPoint.s = s;
	intersectionPoint.t = t;

	return intersectionPoint;
}

IntersectionPoint IntersectionFinder::FindNextPoint(bool flip, std::shared_ptr<IParameterized> surface1, std::shared_ptr<IParameterized> surface2, IntersectionPoint P0, double u1, double u2, double v1, double v2, double s1, double s2, double t1, double t2)
{
	auto dU1 = surface1->GetUDerivativeValue(P0.u, P0.v);
	auto dV1 = surface1->GetVDerivativeValue(P0.u, P0.v);

	auto dU2 = surface2->GetUDerivativeValue(P0.s, P0.t);
	auto dV2 = surface2->GetVDerivativeValue(P0.s, P0.t);

	auto np = dU1.Cross(dV1);
	auto nq = dU2.Cross(dV2);

	auto tangent = np.Cross(nq);
	tangent.Normalize();
	if (flip)
	{
		tangent = -tangent;
	}
	//OutputDebugString(L"tangent: \n");
	//PrintVector(tangent);
	bool outOfRange = false;
	double u, v, s, t;
	for (int i = 1; i <= 8; i *= 2)
	{
		Eigen::VectorXd xk(4);
		Eigen::VectorXd xk1(4);
		xk1 << P0.u, P0.v, P0.s, P0.t;
		//PrintMatrix(xk1);
		u = P0.u;
		v = P0.v;
		s = P0.s;
		t = P0.t;
		float distance = newtonStartDistance / i;
		for (int j = 0; j < 20; j++)
		{
			xk = xk1;
			auto pos1 = surface1->GetValue(u, v);
			auto pos2 = surface2->GetValue(s, t);

			auto dU = surface1->GetUDerivativeValue(u, v);
			auto dV = surface1->GetVDerivativeValue(u, v);
			auto dS = surface2->GetUDerivativeValue(s, t);
			auto dT = surface2->GetVDerivativeValue(s, t);

			MatrixXd F(4, 1);
			F(0, 0) = pos1.x - pos2.x;
			F(1, 0) = pos1.y - pos2.y;
			F(2, 0) = pos1.z - pos2.z;
			F(3, 0) = (pos1 - P0.position).Dot(tangent) - distance;

			//OutputDebugString(L"F: \n");
			//PrintMatrix(F);
			auto norm = F.norm();
			if (F.norm() < newtownMinNorm / i)
			{
				IntersectionPoint intersectionPoint;
				intersectionPoint.position = (surface1->GetValue(u, v) + surface2->GetValue(s, t)) / 2;
				intersectionPoint.u = u;
				intersectionPoint.v = v;
				intersectionPoint.s = s;
				intersectionPoint.t = t;
				intersectionPoint.found = true;
				intersectionPoint.last = false;
				return intersectionPoint;
			}

			MatrixXd dF(4, 4);
			dF(0, 0) = dU.x;
			dF(1, 0) = dU.y;
			dF(2, 0) = dU.z;

			dF(0, 1) = dV.x;
			dF(1, 1) = dV.y;
			dF(2, 1) = dV.z;

			dF(0, 2) = -dS.x;
			dF(1, 2) = -dS.y;
			dF(2, 2) = -dS.z;

			dF(0, 3) = -dT.x;
			dF(1, 3) = -dT.y;
			dF(2, 3) = -dT.z;

			dF(3, 0) = dU.Dot(tangent);
			dF(3, 1) = dV.Dot(tangent);
			dF(3, 2) = 0;
			dF(3, 3) = 0;

			//OutputDebugString(L"dF: \n");
			//PrintMatrix(dF);

			MatrixXd right = dF * xk - F;

			xk1 = dF.completeOrthogonalDecomposition().solve(right);

			u = xk1(0), v = xk1(1), s = xk1(2), t = xk1(3);

			if (u < 0.0 || u > 1.0)
			{
				if (surface1->IsUWrapped())
				{
					//GetInRange(u, 0, 1);
				}
				else
				{
					outOfRange = true;
					break;
				}
			}

			if (v < 0.0 || v > 1.0)
			{
				if (surface1->IsVWrapped())
				{
					//GetInRange(v, 0, 1);
				}
				else
				{
					outOfRange = true;
					break;
				}
			}

			if (s < 0.0 || s > 1.0)
			{
				if (surface2->IsUWrapped())
				{
					//GetInRange(s, 0, 1);
				}
				else
				{
					outOfRange = true;
					break;
				}
			}

			if (t < 0.0 || t > 1.0)
			{
				if (surface2->IsVWrapped())
				{
					//GetInRange(t, 0, 1);
				}
				else
				{
					outOfRange = true;
					break;
				}
			}


			//OutputDebugString(L"xk1: \n");
			//PrintMatrix(xk1);
		}
	}
	if (outOfRange)
	{
		u = std::clamp(u, u1, u2);
		v = std::clamp(v, v1, v2);
		s = std::clamp(s, s1, s2);
		t = std::clamp(t, t1, t2);

		IntersectionPoint intersectionPoint;
		intersectionPoint.position = (surface1->GetValue(u, v) + surface2->GetValue(s, t)) / 2;
		intersectionPoint.u = u;
		intersectionPoint.v = v;
		intersectionPoint.s = s;
		intersectionPoint.t = t;
		intersectionPoint.found = true;
		intersectionPoint.last = true;
		return intersectionPoint;
	}
	IntersectionPoint intersectionPoint;
	intersectionPoint.found = false;
	return intersectionPoint;
}



Pair<double> IntersectionFinder::FindStartingPointFromPosition(std::shared_ptr<IParameterized> surface, DirectX::SimpleMath::Vector3 position)
{
	double bestU = 0.0, bestV = 0.0;
	double bestDistance = DBL_MAX;

	double splitSize = 1.0 / surfaceSplitsOnCursorStart;
	for (double uFirstLevel = 0.0; uFirstLevel <= 1.0; uFirstLevel += splitSize)
	{
		for (double vFirstLevel = 0.0; vFirstLevel <= 1.0; vFirstLevel += splitSize)
		{
			double bestSplitDist = Vector3::DistanceSquared(position, surface->GetValue(uFirstLevel, vFirstLevel));
			double stepSize = splitSize / 4;
			double stepU = uFirstLevel;
			double stepV = vFirstLevel;
			while (stepSize > splitSize / 16)
			{
				double areaDistanceBest = DBL_MAX;
				double areaUBest;
				double areaVBest;

				for (int i = -1; i <= 1; i += 2)
				{
					for (int j = -1; j <= 1; j += 2)
					{
						double u = stepU + stepSize * i;
						double v = stepV + stepSize * j;
						UpdateBest(u, v, areaUBest, areaVBest, areaDistanceBest, surface, position);
					}
				}

				if (areaDistanceBest < bestSplitDist)
				{
					stepU = areaUBest;
					stepV = areaVBest;
					bestSplitDist = areaDistanceBest;
				}
				stepSize /= 2;

			}

			UpdateBest(stepU, stepV, bestU, bestV, bestDistance, surface, position);
		}
	}

	return { bestU, bestV };
}

void IntersectionFinder::UpdateBest(double u, double v, double& bestU, double& bestV, double& bestDistance, std::shared_ptr<IParameterized> surface, Vector3 position)
{
	u = GetInRange(u, 0.0, 1.0);
	v = GetInRange(v, 0.0, 1.0);
	auto surfacePosition = surface->GetValue(u, v);
	auto distance = Vector3::DistanceSquared(position, surfacePosition);
	if (distance < bestDistance)
	{
		bestU = u;
		bestV = v;
		bestDistance = distance;
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

void IntersectionFinder::PrintVector(
	Vector3 v
)
{
	OutputDebugString(L" \n");
	std::wstringstream wss;
	wss << std::fixed << std::setprecision(2) << v.x << " " << v.y << " " << v.z << " ";
	OutputDebugString(wss.str().c_str());

	OutputDebugString(L" \n");

}

void IntersectionFinder::SetCursorPosition(float x, float y, float z)
{
	this->cursorPosition = Vector3(x, y, z);
	this->useCursor = true;
}

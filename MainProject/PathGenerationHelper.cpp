#include "PathGenerationHelper.h"

using namespace DirectX::SimpleMath;

std::vector<DirectX::SimpleMath::Vector3> PathGenerationHelper::RemoveSelfIntersections(std::vector<DirectX::SimpleMath::Vector3> positions)
{
	if (positions.size() <= 1)
	{
		return positions;
	}
	auto newPositions = std::vector<DirectX::SimpleMath::Vector3>();

	auto intersectionPoints = std::vector<std::tuple<int, int, Vector3, Vector3, float>>();
	for (int i = 0; i < positions.size()-1; i++)
	{
		auto A0 = positions[i];
		auto A1 = positions[i+1];

		auto ALength = (A1 - A0).Length();
		for (int j = i + 3; j < positions.size() - 1; j++)
		{
			auto B0 = positions[j];
			auto B1 = positions[j+1];
			auto BLength = (B1 - B0).Length();
			
			auto [A2, B2, distance] = GetDistanceBetweenSegments(A0, A1, B0, B1);
			auto minLength = min(ALength, BLength);

			if (distance < minLength)
			{
				auto AB2 = (B2 - A2);
				auto A = (A1 - A0);
				auto B = (B1 - B0);
				AB2.Normalize();
				A.Normalize();
				B.Normalize();
				auto dotA = A.Dot(AB2);
				auto dotB = B.Dot(AB2);

				if (abs(dotA) < 0.05 && abs(dotB) < 0.05)
				{
					intersectionPoints.push_back(std::make_tuple(i, j, A2, B2, distance));
					i = j + 1;
					break;
				}
			}

			
		}
	}

	if (intersectionPoints.size() == 0)
	{
		newPositions = positions;
	}
	else
	{
		for (int i = 0; i < std::get<0>(intersectionPoints[0]); i++)
		{
			newPositions.push_back(positions[i]);
		}
		for (int i = 1; i < intersectionPoints.size(); i++)
		{
			int start = std::get<1>(intersectionPoints[i - 1]);
			int end = std::get<0>(intersectionPoints[i]);
			for (int j = start; j < end; j++)
			{
				newPositions.push_back(positions[j]);
			}
		}
		for (int i = std::get<1>(intersectionPoints[intersectionPoints.size() - 1]); i < positions.size(); i++)
		{
			newPositions.push_back(positions[i]);
		}
	}
	return newPositions;
}

std::vector<DirectX::SimpleMath::Vector3> PathGenerationHelper::RemoveIntersectionFromCurve(std::vector<DirectX::SimpleMath::Vector3> curve, std::vector<DirectX::SimpleMath::Vector3> intersectingCurve)
{
	if (curve.size() <= 1)
	{
		return curve;
	}
	auto newPositions = std::vector<DirectX::SimpleMath::Vector3>();

	auto intersectionPoints = std::vector<std::tuple<int, int, Vector3, Vector3, float>>();
	for (int i = 0; i < curve.size() - 1; i++)
	{
		auto A0 = curve[i];
		auto A1 = curve[i + 1];

		auto ALength = (A1 - A0).Length();
		for (int j = 0; j < intersectingCurve.size() - 1; j++)
		{
			auto B0 = intersectingCurve[j];
			auto B1 = intersectingCurve[j + 1];
			auto BLength = (B1 - B0).Length();

			auto [A2, B2, distance] = GetDistanceBetweenSegments(A0, A1, B0, B1);
			auto minLength = min(ALength, BLength);

			if (distance < minLength)
			{
				auto AB2 = (B2 - A2);
				auto A = (A1 - A0);
				auto B = (B1 - B0);
				AB2.Normalize();
				A.Normalize();
				B.Normalize();
				auto dotA = A.Dot(AB2);
				auto dotB = B.Dot(AB2);

				if (abs(dotA) < 0.05 && abs(dotB) < 0.05)
				{
					intersectionPoints.push_back(std::make_tuple(i, j, A2, B2, distance));
					break;
				}
			}


		}
	}

	if (intersectionPoints.size() == 0)
	{
		newPositions = curve;
	}
	else
	{
		int start = 0;
		bool adding = true;
		for (size_t i = 0; i < intersectionPoints.size(); i++)
		{
			if (adding)
			{
				int end = std::get<0>(intersectionPoints[i]);
				for (int j = start; j < end; j++)
				{
					newPositions.push_back(curve[j]);
				}
			}
			else
			{
				int start = std::get<0>(intersectionPoints[i]);
			}
			adding = !adding;
		}
		if (adding)
		{
			for (int j = start; j < curve.size()-1; j++)
			{
				newPositions.push_back(curve[j]);
			}
		}
	}
	return newPositions;
}

std::tuple< DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3, float> PathGenerationHelper::GetDistanceBetweenSegments(DirectX::SimpleMath::Vector3 a0, DirectX::SimpleMath::Vector3 a1, DirectX::SimpleMath::Vector3 b0, DirectX::SimpleMath::Vector3 b1)
{
	auto A = a1 - a0;
	auto B = b1 - b0;
	auto magA = A.Length();
	auto magB = B.Length();
	auto _A = A / magA;
	auto 	_B = B / magB;

	auto cross = _A.Cross(_B);
	auto denom = cross.Length();
	denom *= denom;


	if (std::abs(denom) < 1e-6)
	{
		auto d0 = _A.Dot((b0 - a0));

		//# Overlap only possible with clamping
		auto d1 = _A.Dot(b1 - a0);

		//# Is segment B before A ?
		if (d0 <= 0 && 0 >= d1)
		{
			if (abs(d0) < abs(d1))
			{

				return std::make_tuple(a0, b0, (a0 - b0).Length());
			}
			else
			{
				return std::make_tuple(a0, b1, (a0 - b1).Length());
			}
		}
		else if (d0 >= magA && magA <= d1)
		{
			if (abs(d0) < abs(d1))
				return std::make_tuple(a1, b0, (a1 - b0).Length());
			return std::make_tuple(a1, b1, (a1 - b1).Length());
		}
	}
	auto t = (b0 - a0);
	auto detA = Matrix(t, _B, cross).Determinant();
	auto 	detB = Matrix(t, _A, cross).Determinant();

	auto 	t0 = detA / denom;
	auto t1 = detB / denom;

	auto pA = a0 + (_A * t0);// # Projected closest point on segment A
	auto pB = b0 + (_B * t1);// # Projected closest point on segment B

	if (t0 < 0)
	{
		pA = a0;
	}
	else if (t0 > magA)
	{
		pA = a1;
	}

	if (t1 < 0)
		pB = b0;
	else if (t1 > magB)
		pB = b1;


	if ((t0 < 0) || (t0 > magA))
	{
		auto dot = _B.Dot((pA - b0));
		if (dot < 0)
			dot = 0;
		else if (dot > magB)
			dot = magB;
		pB = b0 + (_B * dot);
	}

	if ((t1 < 0) || (t1 > magB))
	{
		auto dot = _A.Dot((pB - a0));
		if (dot < 0)
			dot = 0;
		else if (dot > magA)
			dot = magA;
		pA = a0 + (_A * dot);
	}
	return std::make_tuple(pA, pB, (pA - pB).Length());
}

std::vector<std::tuple<int, int, DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3, float>> PathGenerationHelper::GetCurvesIntersections(std::vector<DirectX::SimpleMath::Vector3> curve1, std::vector<DirectX::SimpleMath::Vector3> curve2)
{
	
	auto newPositions = std::vector<DirectX::SimpleMath::Vector3>();

	auto intersectionPoints = std::vector<std::tuple<int, int, Vector3, Vector3, float>>();
	for (int i = 0; i < curve1.size() - 1; i++)
	{
		auto A0 = curve1[i];
		auto A1 = curve1[i + 1];

		auto ALength = (A1 - A0).Length();
		for (int j = 0; j < curve2.size() - 1; j++)
		{
			auto B0 = curve2[j];
			auto B1 = curve2[j + 1];
			auto BLength = (B1 - B0).Length();

			auto [A2, B2, distance] = GetDistanceBetweenSegments(A0, A1, B0, B1);
			auto minLength = min(ALength, BLength);

			if (distance < minLength)
			{
				auto AB2 = (B2 - A2);
				auto A = (A1 - A0);
				auto B = (B1 - B0);
				AB2.Normalize();
				A.Normalize();
				B.Normalize();
				auto dotA = A.Dot(AB2);
				auto dotB = B.Dot(AB2);

				if (abs(dotA) < 0.05 && abs(dotB) < 0.05)
				{
					intersectionPoints.push_back(std::make_tuple(i, j, A2, B2, distance));
				}
			}


		}
	}

	return intersectionPoints;
}

std::vector<DirectX::SimpleMath::Vector3> PathGenerationHelper::GetPositions(const std::vector<IntersectionPoint>& intersectionPoints)
{
	std::vector<Vector3> positions(intersectionPoints.size());
	for (size_t i = 0; i < intersectionPoints.size(); i++)
	{
		positions[i] = intersectionPoints[i].position;
	}
	return positions;
}

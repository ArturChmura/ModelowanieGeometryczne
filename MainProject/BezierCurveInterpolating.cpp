#include "BezierCurveInterpolating.h"
#include <array>
#include "Helpers.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;
BezierCurveInterpolating::BezierCurveInterpolating(std::vector<std::shared_ptr<Point>> points)
	: IBezierCurve(points, "Bezier Curve Interpolating")
{
	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
}


void BezierCurveInterpolating::CalculatePolynomials()
{
	int n = points.size() - 1;
	bezierPoints = std::vector<std::shared_ptr<VirtualPoint>>(max(4 * n - max(n - 1,0),0));
	if (n <= 0)
	{
		return;
	}
	std::vector<Vector3> Ps(n + 1);
	for (int i = 0; i <= n; i++)
	{
		auto trans = points[i]->GetTranslation();
		Ps[i] = Vector3(trans);
	}

	std::vector<float> distances(n);
	for (int i = 0; i < n; i++)
	{
		Vector3 dP = Ps[i + 1] - Ps[i];
		float dot = dP.Dot(dP);
		distances[i] = max(sqrtf(dot),0.01);
	}

	std::vector<float> alphas(n - 1);
	std::vector<float> betas(n - 1);
	std::vector < Vector3> Rs(n - 1);

	for (int i = 0; i < n - 1; i++)
	{
		alphas[i] = (distances[i]) / (distances[i] + distances[i + 1]);
		betas[i] = (distances[i + 1]) / (distances[i] + distances[i + 1]);
		Rs[i] = 3.0f * ((Ps[i + 2] - Ps[i + 1]) / (distances[i + 1]) - (Ps[i + 1] - Ps[i]) / (distances[i])) / (distances[i] + distances[i + 1]);
	}

	std::vector <float> twos(n - 1);
	for (int i = 0; i < n - 1; i++)
	{
		twos[i] = 2.0f;
	}

	auto cs1N1 = SolveTriDiagonalMatrix(alphas, twos, betas, Rs);
	std::vector<std::array<DirectX::SimpleMath::Vector3, 4>> coef;
	coef = std::vector<std::array<Vector3, 4>>(n + 1);
	coef[0][2] = coef[n][2] = Vector3(0.0f, 0.0f, 0.0f);
	for (int i = 1; i < n; i++)
	{
		coef[i][2] = cs1N1[i - 1];
	}

	for (int i = 0; i <= n; i++)
	{
		coef[i][0] = Ps[i];
	}

	for (int i = 1; i <= n - 1; i++)
	{
		coef[i - 1][3] = (2 * coef[i][2] - 2 * coef[i - 1][2]) / (6.0f * distances[i - 1]);
	}


	for (int i = 1; i <= n - 1; i++)
	{
		coef[i - 1][1] = (coef[i][0] - coef[i - 1][0] - coef[i - 1][2] * powf(distances[i - 1], 2) - coef[i - 1][3] * powf(distances[i - 1], 3)) / distances[i - 1];
	}
	if (n >= 2)
	{
		coef[n - 1][1] = coef[n - 2][1] + 2 * coef[n - 2][2] * distances[n - 2] + 3 * coef[n - 2][3] * powf(distances[n - 2], 2);
	}


	coef[n - 1][3] = (Ps[n] - coef[n - 1][0] - coef[n - 1][1] * distances[n - 1] - coef[n - 1][2] * powf(distances[n - 1], 2)) / powf(distances[n - 1], 3);


	Matrix EtoB = {
		1,1,1,1,
		0, 1.0f / 3, 2.0f / 3, 1,
		0,0,1.0f / 3, 1,
		0,0,0,1
	};
	Vector3 b4;
	for (int i = 0; i < n; i++)
	{
		Vector4 ex = Vector4(coef[i][0].x, coef[i][1].x * distances[i], coef[i][2].x * powf(distances[i], 2), coef[i][3].x * powf(distances[i], 3));
		Vector4 ey = Vector4(coef[i][0].y, coef[i][1].y * distances[i], coef[i][2].y * powf(distances[i], 2), coef[i][3].y * powf(distances[i], 3));
		Vector4 ez = Vector4(coef[i][0].z, coef[i][1].z * distances[i], coef[i][2].z * powf(distances[i], 2), coef[i][3].z * powf(distances[i], 3));

		Vector4 bx = Vector4::Transform(ex, EtoB);
		Vector4 by = Vector4::Transform(ey, EtoB);
		Vector4 bz = Vector4::Transform(ez, EtoB);

		Vector3 b1 = Vector3(bx.x, by.x, bz.x);
		Vector3 b2 = Vector3(bx.y, by.y, bz.y);
		Vector3 b3 = Vector3(bx.z, by.z, bz.z);
		b4 = Vector3(bx.w, by.w, bz.w);
		bezierPoints[i * 3 + 0] = std::make_shared<VirtualPoint>(b1);
		bezierPoints[i * 3 + 1] = std::make_shared<VirtualPoint>(b2);
		bezierPoints[i * 3 + 2] = std::make_shared<VirtualPoint>(b3);
	}
	bezierPoints[4 * n - (n - 1) - 1] = std::make_shared<VirtualPoint>(b4);
}

void BezierCurveInterpolating::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		CalculatePolynomials();
	}
	IBezierCurve::Draw(camera);

	if (bezierRepresentation)
	{
		for (int i = 0; i < bezierPoints.size(); i++)
		{
			bezierPoints[i]->Draw(camera);
		}
	}
}


std::vector<DirectX::SimpleMath::Vector3> BezierCurveInterpolating::GetBezierPoints()
{

	std::vector<Vector3> bezier(bezierPoints.size());
	for (int i = 0; i < bezierPoints.size(); i++)
	{
		bezier[i] = Vector3(bezierPoints[i]->GetTranslation());
	}
	return bezier;
}

void BezierCurveInterpolating::SetRepresentation(bool bezier)
{
	bezierRepresentation = bezier;
	ResetDrawing();
}

void BezierCurveInterpolating::RenderGUI()
{
	IBezierCurve::RenderGUI();

	bool rep = bezierRepresentation;
	if (ImGui::Checkbox("Bezier Representation", &rep))
	{
		SetRepresentation(rep);
	}
}

std::vector<DirectX::SimpleMath::Vector3> BezierCurveInterpolating::GetPolygonChainPoints()
{
	return GetBezierPoints();
}

void BezierCurveInterpolating::Accept(SerializationVisitor& visitor)
{
	visitor.Accept(*this);
}


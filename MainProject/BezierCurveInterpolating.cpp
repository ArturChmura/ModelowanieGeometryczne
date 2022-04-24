#include "BezierCurveInterpolating.h"
#include "Helpers.h"
#include "Vertex.h"

using namespace DirectX::SimpleMath;
BezierCurveInterpolating::BezierCurveInterpolating(std::vector<std::shared_ptr<Point>> points)
	: IBezierCurve(points, "Bezier Curve Interpolating")
{
	this->shaderInfoPolynomial = std::make_shared<ShaderInfoPolynomial>();
	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
}


void BezierCurveInterpolating::CalculatePolynomials()
{
	if (points.size() < 2)
	{
		return;
	}
	int n = points.size() - 1;
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
		distances[i] = sqrtf(dot);
	}

	std::vector<float> alphas(n-1);
	std::vector<float> betas(n-1);
	std::vector < Vector3> Rs(n-1);

	for (int i = 0; i < n - 1; i++)
	{
		alphas[i] = (distances[i]) / (distances[i] + distances[i+1]);
		betas[i] = (distances[i+1]) / (distances[i] + distances[i+1]);
		Rs[i] = 3.0f * ((Ps[i + 2] - Ps[i+1]) / (distances[i+1]) - (Ps[i+1] - Ps[i]) / (distances[i])) / (distances[i] + distances[i+1]);
	}

	std::vector <float> twos(n-1);
	for (int i = 0; i < n-1; i++)
	{
		twos[i] = 2.0f;
	}

	auto cs1N1 = SolveTriDiagonalMatrix(alphas, twos, betas, Rs);
	coef = std::vector<std::array<Vector3,4>>(n+1);
	coef[0][2] = coef[n][2] = Vector3(0.0f, 0.0f, 0.0f);
	for (int i = 1; i < n; i++)
	{
		coef[i][2] = cs1N1[i - 1];
	}

	for (int i = 0; i <= n; i++)
	{
		coef[i][0] = Ps[i];
	}

	for (int i = 1; i <= n-1; i++)
	{
		coef[i - 1][3] = (2 * coef[i][2] - 2 * coef[i - 1][2]) / (6.0f * distances[i - 1]);
	}


	for (int i = 1; i <= n-1; i++)
	{
		coef[i - 1][1] = (coef[i][0] - coef[i - 1][0] - coef[i - 1][2] * powf(distances[i - 1], 2) - coef[i - 1][3] * powf(distances[i - 1], 3)) / distances[i - 1];
	}
	coef[n - 1][1] = coef[n - 2][1] + 2 * coef[n - 2][2] * distances[n - 2] + 3 * coef[n - 2][3] * powf(distances[n - 2], 2);


	coef[n - 1][3] = (Ps[n] - coef[n - 1][0] - coef[n - 1][1] * distances[n - 1] - coef[n - 1][2] * powf(distances[n - 1], 2)) / powf(distances[n - 1], 3);
	
	

	std::vector<Coef> vertices = std::vector<Coef>();

	for (int i = 0; i < n; i++)
	{
		vertices.push_back({ Vector4(coef[i][0].x, coef[i][0].y,coef[i][0].z,distances[i]), coef[i][1],coef[i][2],coef[i][3]});
	}

	std::vector<int> indices = std::vector<int>();
	for (int i = 0; i < (int)vertices.size() - 1; i++)
	{
		indices.push_back(i);
		indices.push_back(i+1);
	}
	indicesCount = indices.size();
	verticesCount = vertices.size();
	if (vertices.size() > 0)
	{
		this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
		this->meshInfo.indexBuffer = DxDevice::instance->CreateVertexBuffer(indices);
	}

}

void BezierCurveInterpolating::Draw(std::shared_ptr<Camera> camera)
{
	CalculatePolynomials();

	meshInfo.SetUpRender();

	shaderInfoPolynomial->SetUpRender();
	shaderInfoPolynomial->SetVertexBuffer(meshInfo.vertexBuffer.get());

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	shaderInfoPolynomial->constantBufferStruct.mvp =
		XMLoadFloat4x4(&v) *
		XMLoadFloat4x4(&p);
	shaderInfoPolynomial->constantBufferStruct.color = XMLoadFloat3(&meshInfo.color);


	shaderInfoPolynomial->CopyConstantBuffers();

	DxDevice::instance->context()->Draw(verticesCount, 0);

}

void BezierCurveInterpolating::UpdateVertices()
{
}


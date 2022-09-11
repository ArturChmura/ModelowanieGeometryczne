#include "SingleBezierSurfaceC0.h"
#include <stdexcept>
#include "ShadersManager.h"
#include "BernsteinHelper.h"

using namespace DirectX::SimpleMath;

SingleBezierSurfaceC0::SingleBezierSurfaceC0(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices)
	: ISingleBezierSurface(points, horizontalSlices, verticalSlices, "Single Bezier Surface C0")
{
	this->points = points;
	this->horizontalSlices = horizontalSlices;
	this->verticalSlices = verticalSlices;

	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}


void SingleBezierSurfaceC0::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		UpdateVertices();
	}
	meshInfo.SetUpRender();

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsSurfaceBezier->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsSurfaceBezier->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(ShadersManager::gsSurfaceBezier->m_geometryShader.get(), nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsSurfaceBezier->SetVertexBuffer(meshInfo.vertexBuffer.get());
	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp = v * p;

	GSSurfaceBezierConstantBuffer gsCB;
	gsCB.mvp = mvp;
	gsCB.slices = horizontalSlices;

	std::vector<Vector3> translations = std::vector<Vector3>(16);

	for (int i = 0; i < 16; i++)
	{
		translations[i] = Vector3(points[i % 4][i / 4]->GetTranslation());
	}
	for (int i = 0; i < 16; i++)
	{
		gsCB.X[i] = translations[i].x;
		gsCB.Y[i] = translations[i].y;
		gsCB.Z[i] = translations[i].z;
	}

	ShadersManager::gsSurfaceBezier->SetConstantBuffer(gsCB);
	DxDevice::instance->context()->Draw(verticalSlices + 1, 0);

	for (int i = 0; i < 16; i++)
	{
		translations[i] = Vector3(points[i / 4][i % 4]->GetTranslation());
	}
	for (int i = 0; i < 16; i++)
	{
		gsCB.X[i] = translations[i].x;
		gsCB.Y[i] = translations[i].y;
		gsCB.Z[i] = translations[i].z;
	}

	gsCB.slices = verticalSlices;

	ShadersManager::gsSurfaceBezier->SetConstantBuffer(gsCB);
	DxDevice::instance->context()->Draw(horizontalSlices + 1, verticalSlices + 1);

}
void SingleBezierSurfaceC0::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<SingleBezierSurfaceC0>());
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC0::GetValue(float u, float v)
{
	std::array<Vector3, 4> vertical;
	for (int i = 0; i < 4; i++)
	{
		std::array<Vector3, 4> horizontal;
		for (int j = 0; j < 4; j++)
		{
			horizontal[j] = Vector3(points[i][j]->GetTranslation());
		}
		auto v = BernstainHelper::DeCasteljeu(horizontal, u, 4);
		vertical[i] = v;
	}
	auto result = BernstainHelper::DeCasteljeu(vertical, v, 4);
	return result;
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC0::GetUDerivativeValue(float u, float v)
{
	std::array<Vector3, 4> horizontal;
	for (int j = 0; j < 4; j++)
	{
		std::array<Vector3, 4> vertical;
		for (int i = 0; i < 4; i++)
		{
			vertical[i] = Vector3(points[i][j]->GetTranslation());
		}
		auto vector = BernstainHelper::DeCasteljeu(vertical, v, 4);
		horizontal[j] = vector;
	}
	auto result = BernstainHelper::dU(horizontal, u);
	return result;
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC0::GetVDerivativeValue(float u, float v)
{
	std::array<Vector3, 4> vertical;
	for (int i = 0; i < 4; i++)
	{
		std::array<Vector3, 4> horizontal;
		for (int j = 0; j < 4; j++)
		{
			horizontal[j] = Vector3(points[i][j]->GetTranslation());
		}
		auto vector = BernstainHelper::DeCasteljeu(horizontal, u, 4);
		vertical[i] = vector;
	}
	auto result = BernstainHelper::dU(vertical, v);
	return result;
}

bool SingleBezierSurfaceC0::IsUWrapped()
{
	return false;
}

bool SingleBezierSurfaceC0::IsVWrapped()
{
	return false;
}

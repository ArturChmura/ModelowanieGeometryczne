#include "SingleBezierSurfaceC0.h"
#include <stdexcept>
#include "ShadersManager.h"
#include "BernsteinHelper.h"

using namespace DirectX::SimpleMath;

SingleBezierSurfaceC0::SingleBezierSurfaceC0(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices,
	int rowIndex, int columnIndex,
	int rowCount, int columnCount
)
	: ISingleBezierSurface(points, horizontalSlices, verticalSlices, rowIndex, columnIndex, rowCount, columnCount , "Single Bezier Surface C0")
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
	ZeroMemory(&gsCB, sizeof(gsCB));
	gsCB.mvp = mvp;
	gsCB.slices = horizontalSlices;
	gsCB.flipped = 0.0f;

	std::vector<Vector3> translations = std::vector<Vector3>(16);

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
	gsCB.rowIndex = rowIndex;
	gsCB.columnIndex = columnIndex;
	gsCB.rowCount = rowCount;
	gsCB.columnCount = columnCount;

	if (filterTextureView)
	{
		ShadersManager::gsSurfaceBezier->SetFilterTexture(filterTextureView);
		gsCB.filter = true;
	}
	ShadersManager::gsSurfaceBezier->SetConstantBuffer(gsCB);
	DxDevice::instance->context()->Draw(verticalSlices + 1, 0);

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

	gsCB.slices = verticalSlices;
	gsCB.flipped = 1.0f;

	ShadersManager::gsSurfaceBezier->SetConstantBuffer(gsCB);
	DxDevice::instance->context()->Draw(horizontalSlices + 1, verticalSlices + 1);

}
void SingleBezierSurfaceC0::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<SingleBezierSurfaceC0>());
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC0::GetValue(double u, double v)
{
	std::array<Vector3, 4> vertical;
	for (int i = 0; i < 4; i++)
	{
		std::array<Vector3, 4> horizontal;
		for (int j = 0; j < 4; j++)
		{
			horizontal[j] = Vector3(points[i][j]->GetTranslation());
		}
		auto value = BernstainHelper::DeCasteljeu(horizontal, v, 4);
		vertical[i] = value;
	}
	auto result = BernstainHelper::DeCasteljeu(vertical, u, 4);
	return result;
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC0::GetUDerivativeValue(double u, double v)
{
	 std::array<Vector3, 4> vertical;
	for (int i = 0; i < 4; i++)
	{
		std::array<Vector3, 4> horizontal;
		for (int j = 0; j < 4; j++)
		{
			horizontal[j] = Vector3(points[i][j]->GetTranslation());
		}
		auto vector = BernstainHelper::DeCasteljeu(horizontal, v, 4);
		vertical[i] = vector;
	}
	auto result = BernstainHelper::dU(vertical, u);
	return result;
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC0::GetVDerivativeValue(double u, double v)
{
	std::array<Vector3, 4> horizontal;
	for (int j = 0; j < 4; j++)
	{
		std::array<Vector3, 4> vertical;
		for (int i = 0; i < 4; i++)
		{
			vertical[i] = Vector3(points[i][j]->GetTranslation());
		}
		auto vector = BernstainHelper::DeCasteljeu(vertical, u, 4);
		horizontal[j] = vector;
	}
	auto result = BernstainHelper::dU(horizontal, v);
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

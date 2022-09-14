#include "SingleBezierSurfaceC2.h"
#include "ShadersManager.h"
#include "imgui.h"
#include "BernsteinHelper.h"

using namespace DirectX::SimpleMath;
SingleBezierSurfaceC2::SingleBezierSurfaceC2(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices,
	int rowIndex, int columnIndex,
	int rowCount, int columnCount)
	: ISingleBezierSurface(points, horizontalSlices, verticalSlices, rowIndex, columnIndex, rowCount, columnCount, "Single Bezier Surface C2")
{
	this->points = points;
	this->horizontalSlices = horizontalSlices;
	this->verticalSlices = verticalSlices;

	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}

void SingleBezierSurfaceC2::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		UpdateVertices();
	}
	meshInfo.SetUpRender();

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsSurfaceBezier->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsSurfaceBezier->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(ShadersManager::gsBezierSurfaceC2->m_geometryShader.get(), nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsSurfaceBezier->SetVertexBuffer(meshInfo.vertexBuffer.get());
	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp = v * p;

	GSBezierSurfaceC2ConstantBuffer gsCB;
	ZeroMemory(&gsCB, sizeof(gsCB));
	gsCB.mvp = mvp;
	gsCB.slices = verticalSlices;
	gsCB.rowIndex = rowIndex;
	gsCB.columnIndex = columnIndex;
	gsCB.rowCount = rowCount;
	gsCB.columnCount = columnCount;
	gsCB.flipped = 0.0f;
	if (filterTextureView)
	{
		ShadersManager::gsBezierSurfaceC2->SetFilterTexture(filterTextureView);
		gsCB.filter = true;
	}
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

	ShadersManager::gsBezierSurfaceC2->SetConstantBuffer(gsCB);
	DxDevice::instance->context()->Draw(horizontalSlices + 1, 0);

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

	gsCB.slices = horizontalSlices;
	gsCB.flipped = 1.0f;

	ShadersManager::gsBezierSurfaceC2->SetConstantBuffer(gsCB);
	DxDevice::instance->context()->Draw(verticalSlices + 1, horizontalSlices + 1);

}


void SingleBezierSurfaceC2::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<SingleBezierSurfaceC2>());
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC2::GetValue(float u, float v)
{
	std::array<Vector3, 4> vertical;
	for (int i = 0; i < 4; i++)
	{
		std::array<Vector3, 4> horizontal;
		for (int j = 0; j < 4; j++)
		{
			horizontal[j] = Vector3(points[i][j]->GetTranslation());
		}
		auto v = BernstainHelper::DeBoor(horizontal, u);
		vertical[i] = v;
	}
	auto result = BernstainHelper::DeBoor(vertical, v);
	return result;
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC2::GetUDerivativeValue(float u, float v)
{
	std::array<Vector3, 4> horizontal;
	for (int j = 0; j < 4; j++)
	{
		std::array<Vector3, 4> vertical;
		for (int i = 0; i < 4; i++)
		{
			vertical[i] = Vector3(points[i][j]->GetTranslation());
		}
		auto vector = BernstainHelper::DeBoor(vertical, v);
		horizontal[j] = vector;
	}
	auto result = BernstainHelper::DeBoorDerivative(horizontal, u);
	return result;
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC2::GetVDerivativeValue(float u, float v)
{
	std::array<Vector3, 4> vertical;
	for (int i = 0; i < 4; i++)
	{
		std::array<Vector3, 4> horizontal;
		for (int j = 0; j < 4; j++)
		{
			horizontal[j] = Vector3(points[i][j]->GetTranslation());
		}
		auto vector = BernstainHelper::DeBoor(horizontal, u);
		vertical[i] = vector;
	}
	auto result = BernstainHelper::DeBoorDerivative(vertical, v);
	return result;
}

bool SingleBezierSurfaceC2::IsUWrapped()
{
	return false;
}

bool SingleBezierSurfaceC2::IsVWrapped()
{
	return false;
}

#include "SingleBezierSurfaceC2.h"
#include "ShadersManager.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;
SingleBezierSurfaceC2::SingleBezierSurfaceC2(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices)
	: ISingleBezierSurface(points, horizontalSlices, verticalSlices, "Single Bezier Surface C2")
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

	ShadersManager::gsBezierSurfaceC2->SetConstantBuffer(gsCB);
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

	ShadersManager::gsBezierSurfaceC2->SetConstantBuffer(gsCB);
	DxDevice::instance->context()->Draw(horizontalSlices + 1, verticalSlices + 1);

}


void SingleBezierSurfaceC2::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<SingleBezierSurfaceC2>());
}

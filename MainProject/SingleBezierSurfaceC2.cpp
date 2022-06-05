#include "SingleBezierSurfaceC2.h"
#include "ShadersManager.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;
SingleBezierSurfaceC2::SingleBezierSurfaceC2(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices)
	: IModel("Single Bezier Surface")
{
	this->points = points;
	this->horizontalSlices = horizontalSlices;
	this->verticalSlices = verticalSlices;

	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}

void SingleBezierSurfaceC2::SetDrawPolygonChain(bool drawPolygonChain)
{
	this->drawPolygonChain = drawPolygonChain;
}

std::vector<std::shared_ptr<IModel>> SingleBezierSurfaceC2::GetContainingModels()
{
	auto models = std::vector<std::shared_ptr<IModel>>();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			models.push_back(points[i][j]);
		}
	}
	return models;
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

void SingleBezierSurfaceC2::DrawPolygonChain(std::shared_ptr<Camera> camera)
{
	std::vector<Vector3> positions(4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			positions[j] = Vector3(points[i][j]->GetTranslation());
		}
		PolygonalChain::Draw(camera, positions);
		for (int j = 0; j < 4; j++)
		{
			positions[j] = Vector3(points[j][i]->GetTranslation());
		}
		PolygonalChain::Draw(camera, positions);
	}
}

void SingleBezierSurfaceC2::UpdateVertices()
{
	resetDrawing = false;

	std::vector<SurfaceBezierIn> vertices = std::vector<SurfaceBezierIn>();

	std::vector<Vector3> translations = std::vector<Vector3>(16);


	float step = 1.0f / verticalSlices;
	for (float t = 0; t <= 1.0f + step / 2; t += step)
	{
		SurfaceBezierIn input;
		input.UV = Vector4(t, 0, 0, 0);

		vertices.push_back(input);
	}

	step = 1.0f / horizontalSlices;
	for (float t = 0; t <= 1.0f + step / 2; t += step)
	{
		SurfaceBezierIn input;
		input.UV = Vector4(t, 0, 0, 0);

		vertices.push_back(input);
	}

	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
}
void SingleBezierSurfaceC2::OnRemovedFromScene()
{
	for (auto points2 : points)
	{
		for (auto point : points2)
		{
			point->onRemovedFromSceneCallback.Remove(id);
		}
	}
}

void SingleBezierSurfaceC2::RenderGUI()
{
	if (
		ImGui::DragInt("Horizontal Slices Count", &horizontalSlices, 1, 1, 255) ||
		ImGui::DragInt("Vertical Slices Count", &verticalSlices, 1, 1, 255)
		)
	{
		horizontalSlices = min(max(horizontalSlices, 1), 255);
		verticalSlices = min(max(verticalSlices, 1), 255);
		resetDrawing = true;
	}
}

void SingleBezierSurfaceC2::SetScale(float x, float y, float z)
{
}

void SingleBezierSurfaceC2::Scale(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC2::GetScale()
{
	return DirectX::SimpleMath::Vector3();
}

void SingleBezierSurfaceC2::ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale)
{
}

void SingleBezierSurfaceC2::SetTranslation(float x, float y, float z)
{
}

void SingleBezierSurfaceC2::Translate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector4 SingleBezierSurfaceC2::GetTranslation()
{
	return DirectX::SimpleMath::Vector4();
}

void SingleBezierSurfaceC2::SetRotation(float x, float y, float z)
{
}

void SingleBezierSurfaceC2::Rotate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 SingleBezierSurfaceC2::GetRotation()
{
	return DirectX::SimpleMath::Vector3();
}

void SingleBezierSurfaceC2::RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{
}

void SingleBezierSurfaceC2::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	meshInfo.color = color;

}

void SingleBezierSurfaceC2::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(*this);
}

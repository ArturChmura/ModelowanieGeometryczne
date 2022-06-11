#include "ISingleBezierSurface.h"
#include "vsSurfaceBezier.h"
#include "imgui.h"
using namespace DirectX::SimpleMath;

ISingleBezierSurface::ISingleBezierSurface(std::array<std::array<std::shared_ptr<Point>, 4>, 4> points, int horizontalSlices, int verticalSlices, std::string name)
	: IUnmovableModel("Single Bezier Surface")
{
	this->points = points;
	this->horizontalSlices = horizontalSlices;
	this->verticalSlices = verticalSlices;

	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}

void ISingleBezierSurface::SetDrawPolygonChain(bool drawPolygonChain)
{
	this->drawPolygonChain = drawPolygonChain;
}

std::vector<std::shared_ptr<IModel>> ISingleBezierSurface::GetContainingModels()
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


void ISingleBezierSurface::DrawPolygonChain(std::shared_ptr<Camera> camera)
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

void ISingleBezierSurface::UpdateVertices()
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

void ISingleBezierSurface::OnRemovedFromScene()
{
	for (auto points2 : points)
	{
		for (auto point : points2)
		{
			point->onRemovedFromSceneCallback.Remove(id);
		}
	}
}


void ISingleBezierSurface::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	meshInfo.color = color;
}

void ISingleBezierSurface::RenderGUI()
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


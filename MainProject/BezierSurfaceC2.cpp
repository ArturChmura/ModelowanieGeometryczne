#include "BezierSurfaceC2.h"
#include "ImGui/imgui.h"

using namespace DirectX::SimpleMath;

BezierSurfaceC2::BezierSurfaceC2(int horizontalSlicesCount, int verticalSlicesCount, float x, float y, bool cylinder, DirectX::SimpleMath::Vector3 center)
{
	this->horizontalSlicesCount = horizontalSlicesCount;
	this->verticalSlicesCount = verticalSlicesCount;

	int pointsHorizontalCount = horizontalSlicesCount + 3;
	int pointsVerticalCount = cylinder ? verticalSlicesCount : verticalSlicesCount  + 3;

	points = std::vector<std::vector<std::shared_ptr<Point>>>(pointsVerticalCount);
	for (int i = 0; i < pointsVerticalCount; i++)
	{
		points[i] = std::vector<std::shared_ptr<Point>>(pointsHorizontalCount);
	}

	for (int i = 0; i < pointsVerticalCount; i++)
	{
		for (int j = 0; j < pointsHorizontalCount; j++)
		{
			Vector3 position;
			if (cylinder)
			{
				float dx = x / (pointsHorizontalCount - 3);
				float lx = -dx * (pointsHorizontalCount - 1) / 2.0f;
				float xx = lx + j * dx;
				float alphaStep = DirectX::XM_2PI / pointsVerticalCount;
				float yy = sinf(alphaStep * i) * y;
				float zz = cosf(alphaStep * i) * y;
				position = Vector3(xx, yy, zz) + center;
			}
			else
			{
				float dx = x / (pointsHorizontalCount - 3);
				float dy = y / (pointsVerticalCount - 3);
				float lx = -dx * (pointsHorizontalCount - 1) / 2.0f;
				float by = -dy * (pointsVerticalCount - 1) / 2.0f;
				position = Vector3(lx + j * dx, by + i * dy, 0);
				position += center;
			}
			auto point = std::make_shared<Point>(position);
			points[i][j] = point;
		}
	}

	for (int i = 0; i < verticalSlicesCount; i ++)
	{
		for (int j = 0; j < horizontalSlicesCount; j ++)
		{
			const int m = pointsVerticalCount;
			std::array<std::array<std::shared_ptr<Point>, 4>, 4> singleSurfacePoints =
			{ {
				{points[(i + 0) % m][j], points[(i + 0) % m][j + 1], points[(i + 0) % m][j + 2], points[(i + 0) % m][j + 3]},
				{points[(i + 1) % m][j], points[(i + 1) % m][j + 1], points[(i + 1) % m][j + 2], points[(i + 1) % m][j + 3]},
				{points[(i + 2) % m][j], points[(i + 2) % m][j + 1], points[(i + 2) % m][j + 2], points[(i + 2) % m][j + 3]},
				{points[(i + 3) % m][j], points[(i + 3) % m][j + 1], points[(i + 3) % m][j + 2], points[(i + 3) % m][j + 3]}
			} };
			auto singleSurface = std::make_shared<SingleBezierSurfaceC2>(singleSurfacePoints);
			singleSurfaces.push_back(singleSurface);
		}
	}
}

void BezierSurfaceC2::Draw(std::shared_ptr<Camera> camera)
{
	for (int i = 0; i < singleSurfaces.size(); i++)
	{
		singleSurfaces[i]->Draw(camera);
	}

	if (drawPolygonChain)
	{
		std::vector<Vector3> positions(horizontalSlicesCount + 3);
		for (int i = 0; i < points.size(); i++)
		{
			for (int j = 0; j < points[i].size(); j++)
			{
				positions[j] = Vector3(points[i][j]->GetTranslation());
			}
			PolygonalChain::Draw(camera, positions);
		}
		positions = std::vector<Vector3>(verticalSlicesCount + 3);
		for (int j = 0; j < horizontalSlicesCount + 3; j++)
		{
			for (int i = 0; i < verticalSlicesCount +3; i++)
			{
				positions[i] = Vector3(points[i % points.size()][j]->GetTranslation());
			}
			PolygonalChain::Draw(camera, positions);
		}
	}
}

void BezierSurfaceC2::RenderGUI()
{
	IModel::RenderGUI();
	static int selectedIndex = -1;
	if (ImGui::BeginListBox("##ObjectsListBox", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < this->singleSurfaces.size(); i++)
		{
			auto model = this->singleSurfaces[i];
			const bool is_selected = i == selectedIndex;

			if (ImGui::Selectable((model->name + "##" + std::to_string(model->id)).c_str(), is_selected))
			{
				if (is_selected)
				{
					selectedIndex = -1;
					this->singleSurfaces[i]->ChangeColor(selectedColor);
				}
				else
				{
					if (selectedIndex >= 0)
					{
						this->singleSurfaces[selectedIndex]->ChangeColor(selectedColor);
					}
					selectedIndex = i;
					this->singleSurfaces[i]->ChangeColor(Vector3(1, 0, 0));
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	if (ImGui::Checkbox("Draw polygon chain", &drawPolygonChain))
	{
		for (auto single : singleSurfaces)
		{
			single->SetDrawPolygonChain(drawPolygonChain);
		}
	}
	if (selectedIndex >= 0)
	{
		this->singleSurfaces[selectedIndex]->RenderGUI();
	}
}

std::vector<std::vector<std::shared_ptr<Point>>> BezierSurfaceC2::GetPoints()
{
	return points;
}



void BezierSurfaceC2::SetScale(float x, float y, float z)
{
}

void BezierSurfaceC2::Scale(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 BezierSurfaceC2::GetScale()
{
	return DirectX::SimpleMath::Vector3();
}

void BezierSurfaceC2::ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale)
{
}

void BezierSurfaceC2::SetTranslation(float x, float y, float z)
{
}

void BezierSurfaceC2::Translate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector4 BezierSurfaceC2::GetTranslation()
{
	return DirectX::SimpleMath::Vector4();
}

void BezierSurfaceC2::SetRotation(float x, float y, float z)
{
}

void BezierSurfaceC2::Rotate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 BezierSurfaceC2::GetRotation()
{
	return DirectX::SimpleMath::Vector3();
}

void BezierSurfaceC2::RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{
}

void BezierSurfaceC2::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	for (auto single : singleSurfaces)
	{
		single->ChangeColor(color);
	}
}

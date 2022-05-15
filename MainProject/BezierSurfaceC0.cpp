#include "BezierSurfaceC0.h"
#include "ImGui/imgui.h"

using namespace DirectX::SimpleMath;
BezierSurfaceC0::BezierSurfaceC0(
	int horizontalSlicesCount,
	int verticalSlicesCount,
	float x,
	float y,
	bool cylinder,
	DirectX::SimpleMath::Vector3 center)
	: IModel("Bezier Surface C0")
{
	this->horizontalSlicesCount = horizontalSlicesCount;
	this->verticalSlicesCount = verticalSlicesCount;

	int pointsHorizontalCount = horizontalSlicesCount * 3 + 1;
	int pointsVerticalCount = cylinder ? verticalSlicesCount * 3 : verticalSlicesCount * 3 + 1;
	int pointsCount = pointsHorizontalCount * pointsVerticalCount;

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
				float xx = -x / 2.0f + j * (x / (pointsHorizontalCount-1));
				float alphaStep = DirectX::XM_2PI / pointsVerticalCount;
				float yy = sinf(alphaStep * i) * y;
				float zz = cosf(alphaStep * i) * y;
				position = Vector3(xx, yy, zz) + center;
			}
			else
			{
				Vector3 leftBottom = Vector3(-x / 2.0f + j * (x / (pointsHorizontalCount - 1)), -y / 2.0f + i * (y / (pointsVerticalCount - 1)), 0);
				leftBottom += center;
				position = leftBottom;
			}
			auto point = std::make_shared<Point>(position);
			points[i][j] = point;
		}
	}

	for (int i = 0; i < pointsVerticalCount - 1; i += 3)
	{
		for (int j = 0; j < pointsHorizontalCount - 1; j += 3)
		{
			std::array<std::array<std::shared_ptr<Point>, 4>, 4> singleSurfacePoints =
			{ {
				{points[i + 0][j], points[i + 0][j + 1], points[i + 0][j + 2], points[i + 0][j + 3]},
				{points[i + 1][j], points[i + 1][j + 1], points[i + 1][j + 2], points[i + 1][j + 3]},
				{points[i + 2][j], points[i + 2][j + 1], points[i + 2][j + 2], points[i + 2][j + 3]},
				{points[(i + 3) % pointsVerticalCount][j], points[(i + 3) % pointsVerticalCount][j + 1], points[(i + 3) % pointsVerticalCount][j + 2], points[(i + 3) % pointsVerticalCount][j + 3]}
			} };
			auto singleSurface = std::make_shared<SingleBezierSurfaceC0>(singleSurfacePoints);
			singleSurfaces.push_back(singleSurface);
		}
	}

}

void BezierSurfaceC0::Draw(std::shared_ptr<Camera> camera)
{
	for (int i = 0; i < singleSurfaces.size(); i++)
	{
		singleSurfaces[i]->Draw(camera);
	}

	if (drawPolygonChain)
	{
		std::vector<Vector3> positions(horizontalSlicesCount *3 + 1);
		for (int i = 0; i < points.size(); i++)
		{
			for (int j = 0; j < points[i].size(); j++)
			{
				positions[j] = Vector3(points[i][j]->GetTranslation());
			}
			PolygonalChain::Draw(camera, positions);
		}
		positions = std::vector<Vector3>(verticalSlicesCount *3 + 1);
		for (int j = 0; j < horizontalSlicesCount * 3 + 1; j++)
		{
			for (int i = 0; i < verticalSlicesCount * 3 + 1; i++)
			{
				positions[i] = Vector3(points[i % points.size()][j]->GetTranslation());
			}
			PolygonalChain::Draw(camera, positions);
		}
	}
}

void BezierSurfaceC0::RenderGUI()
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

std::shared_ptr<IModel> BezierSurfaceC0::SelectFromScreenCoords(float x, float y, DirectX::SimpleMath::Matrix VP)
{
	for (auto horizontalPoints : points)
	{
		for (auto point : horizontalPoints)
		{
			auto modelPtr = point->SelectFromScreenCoords(x, y, VP);
			if (modelPtr)
			{
				return modelPtr;
			}
		}
		
	}
	return nullptr;
}

std::vector<std::vector<std::shared_ptr<Point>>> BezierSurfaceC0::GetPoints()
{
	return points;
}

void BezierSurfaceC0::SetScale(float x, float y, float z)
{
}

void BezierSurfaceC0::Scale(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 BezierSurfaceC0::GetScale()
{
	return DirectX::SimpleMath::Vector3();
}

void BezierSurfaceC0::ScaleFromPoint(DirectX::SimpleMath::Vector4 point, DirectX::XMFLOAT3 scale)
{
}

void BezierSurfaceC0::SetTranslation(float x, float y, float z)
{
}

void BezierSurfaceC0::Translate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector4 BezierSurfaceC0::GetTranslation()
{
	return DirectX::SimpleMath::Vector4();
}

void BezierSurfaceC0::SetRotation(float x, float y, float z)
{
}

void BezierSurfaceC0::Rotate(float x, float y, float z)
{
}

DirectX::SimpleMath::Vector3 BezierSurfaceC0::GetRotation()
{
	return DirectX::SimpleMath::Vector3();
}

void BezierSurfaceC0::RotateFromPoint(DirectX::SimpleMath::Vector4 globalPoint, DirectX::XMFLOAT3 ratation)
{
}

void BezierSurfaceC0::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	for (auto single: singleSurfaces)
	{
		single->ChangeColor(color);
	}
}

#include "StraightCurveInterpolating.h"
#include "imgui.h"
#include <algorithm>
#include "ShadersManager.h"
#include <nfd.h>
#include "CurveToToolPathsSerializer.h"

using namespace DirectX::SimpleMath;

StraightCurveInterpolating::StraightCurveInterpolating(std::vector<std::shared_ptr<Point>> points)
	: IUnmovableModel("Straight Curve Interpolating")
{
	for (size_t i = 0; i < points.size(); i++)
	{
		AddPoint(points[i]);
	}
	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
}

void StraightCurveInterpolating::UpdateVertices()
{
	resetDrawing = false;

	std::vector<VSConstColorIn> vertices = std::vector<VSConstColorIn>();
	for (auto point : points)
	{
		VSConstColorIn in;

		auto position = Vector3(point->GetTranslation());
		in.pos = position;

		vertices.push_back(in);
	}
	std::vector<int> indices(max(points.size() * 2 - 2, 0));
	for (int i = 1; i < points.size(); i++)
	{
		indices[2 * i - 1 - 1] = i - 1;
		indices[2 * i - 1] = i;
	}
	this->verticesCount = vertices.size();
	this->indicesCount = indices.size();

	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	this->meshInfo.indexBuffer = DxDevice::instance->CreateIndexBuffer(indices);

}

void StraightCurveInterpolating::Draw(std::shared_ptr<Camera> camera)
{
	if (resetDrawing)
	{
		UpdateVertices();
	}

	meshInfo.SetUpRender();

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsConstColor->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsConstColor->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(nullptr, nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsConstColor->SetVertexBuffer(meshInfo.vertexBuffer.get());

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp = v * p;

	ShadersManager::vsConstColor->SetConstantBuffer(mvp);

	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	DxDevice::instance->context()->DrawIndexed(indicesCount, 0,0);
}

void StraightCurveInterpolating::AddPoint(std::shared_ptr<Point> point)
{
	this->points.push_back(point);
	point->onModelChangeCallback.Add([this](std::shared_ptr<Point> point) { this->ResetDrawing(); }, id);
	point->onRemovedFromSceneCallback.Add([this](std::shared_ptr < Point> point) { RemovePoint(point->id); }, id);
	ResetDrawing();
}

void StraightCurveInterpolating::AddPointToBeggining(std::shared_ptr<Point> point)
{
	this->points.insert(this->points.begin(), point);
	point->onModelChangeCallback.Add([this](std::shared_ptr<Point> point) { this->ResetDrawing(); }, id);
	point->onRemovedFromSceneCallback.Add([this](std::shared_ptr < Point> point) { RemovePoint(point->id); }, id);
	ResetDrawing();
}

void StraightCurveInterpolating::RemovePoint(int pointId)
{
	auto iter = std::find_if(points.begin(), points.end(), [pointId](std::shared_ptr<Point> point) {
		return point->id == pointId;
		});
	if (iter == points.end())
	{
		return;
	}
	auto model = (*iter);
	points.erase(iter, iter + 1);

	model->onModelChangeCallback.Remove(id);
	model->onRemovedFromSceneCallback.Remove(id);

	ResetDrawing();
}

void StraightCurveInterpolating::ResetDrawing()
{
	resetDrawing = true;
}


void StraightCurveInterpolating::OnRemovedFromScene()
{
	std::vector<int> toDelete;
	for (auto point : points)
	{
		toDelete.push_back(point->id);
	}
	for (auto id : toDelete)
	{
		RemovePoint(id);
	}
	Point::onSelectCallback.Remove(id);
	Point::onAddedToSceneCallback.Remove(id);
}

void StraightCurveInterpolating::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	meshInfo.color = color;
}

void StraightCurveInterpolating::RenderGUI()
{
	IModel::RenderGUI();
	if (isAddingMode)
	{
		if (ImGui::Button("Stop adding vertices"))
		{
			isAddingMode = false;
			Point::onSelectCallback.Remove(id);
			Point::onAddedToSceneCallback.Remove(id);
		}
	}
	else
	{
		if (ImGui::Button("Start adding vertices"))
		{
			isAddingMode = true;
			auto callback = [this](std::shared_ptr<Point> point) {
				AddPoint(point);
			};
			Point::onSelectCallback.Add(callback, id);
			Point::onAddedToSceneCallback.Add(callback, id);
		}
	}
	if (ImGui::Button("Save as Tool Path"))
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog(NULL, NULL, &outPath);
		if (result == NFD_OKAY) {

			auto success = CurveToToolPathsSerializer::Serialize(this->points, outPath);
			if (success)
			{
				ImGui::OpenPopup("Successfull save");
			}


		}
		else {
			ImGui::OpenPopup("Error popup");
		}
		free(outPath);
	}

}

std::vector<std::shared_ptr<IModel>> StraightCurveInterpolating::GetContainingModels()
{
	std::vector<std::shared_ptr<IModel>> models(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		models[i] = points[i];
	}

	return models;
}

void StraightCurveInterpolating::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<StraightCurveInterpolating>());
}

bool StraightCurveInterpolating::Serializable()
{
	return true;
}

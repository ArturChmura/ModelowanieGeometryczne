#include "Scene.h"
#include "Torus.h"
#include "Point.h"
#include "SimpleMath.h"
#include "BezierCurve.h"
using namespace DirectX::SimpleMath;
Scene::Scene(std::shared_ptr<Coursor3d> cursor, SIZE windowSize)
{
	this->cursor = cursor;
	this->composite = std::make_shared<CompositeModel>();
	this->windowSize = windowSize;
}
void Scene::AddCamera(std::shared_ptr<Camera> camera)
{
	cameras.push_back(camera);
	if (cameras.size() == 1)
	{
		activeCamera = camera;
	}
}

void Scene::SetActiveCamera(std::shared_ptr<Camera> camera)
{
	if (std::find(cameras.begin(), cameras.end(), camera) == cameras.end())
	{
		cameras.push_back(camera);
	}
	activeCamera = camera;
}


std::shared_ptr<Torus> Scene::AddTorus()
{
	auto torus = std::make_shared<Torus>(5.0f, 2.0f, 20, 20);
	torus->SetTranslation(cursor->translation.x, cursor->translation.y, cursor->translation.z);
	AddModel(torus);
	return torus;
}

std::shared_ptr<Point> Scene::AddPoint()
{
	auto point = std::make_shared<Point>(cursor->translation);
	point->onRemovedFromSceneCallback.push_back({ -1,[this](Point* point) {
		auto newEnd = std::remove_if(points.begin(), points.end(), [point](std::shared_ptr<Point> listpoint) {return listpoint->id == point->id; });
		points.erase(newEnd, points.end());
		return;
			} });
	points.push_back(point);
	AddModel(point);
	return point;
}

void Scene::AddModel(std::shared_ptr<IModel> model)
{
	models.push_back(model);
	model->OnAddedToScene();
}

void Scene::AddBezierCurveFromSelectedPoints()
{
	auto selectedPointsSh = GetSelectedPoints();
	std::vector<Point*> selectedPoints;
	for (auto point : selectedPointsSh)
	{
		selectedPoints.push_back(point.get());
	}
	auto bezier = std::make_shared<BezierCurve>(selectedPoints);
	AddModel(bezier);
}

std::vector<std::shared_ptr<Point>> Scene::GetSelectedPoints()
{
	std::vector<std::shared_ptr<Point>> selectedPoints;
	for (auto point : points)
	{
		if (composite->modelsMap.contains(point->id))
		{
			selectedPoints.push_back(point);
		}
	}
	return selectedPoints;
}

void Scene::DeleteModel(int modelId)
{
	auto modelIter = std::find_if(models.begin(), models.end(), [modelId](std::shared_ptr<IModel> model){ return model->id == modelId; });

	auto model = (*modelIter);
	model->OnRemovedFromScene();
	auto new_end = std::remove_if(models.begin(), models.end(),
		[modelId](const std::shared_ptr<IModel>& model)
		{ return model->id == modelId; });
	models.erase(new_end, models.end());

	auto new_end2 = std::remove_if(points.begin(), points.end(),
		[modelId](const std::shared_ptr<IModel>& model)
		{ return model->id == modelId; });
	points.erase(new_end2, points.end());

}

void Scene::ChangeSelection(int modelId)
{
	const bool is_selected = IsSelcted(modelId);
	auto it = std::find_if(models.begin(), models.end(), [modelId](const auto& m) { return m->id == modelId; });
	auto model = *it;
	if (is_selected)
	{
		composite->RemoveModel(model);
		model->OnDeselect();
	}
	else
	{
		composite->AddModel(model);
		model->OnSelect();
	}

	if (composite->modelsMap.size() == 0)
	{
		selectedModel = nullptr;
	}
	else if (composite->modelsMap.size() == 1)
	{
		selectedModel = composite->modelsMap.begin()->second;
	}
	else
	{
		selectedModel = composite;
	}
}

bool Scene::IsSelcted(int modelId)
{
	return composite->modelsMap.contains(modelId);
}

void Scene::ChangeSelectionFromScreenCoords(float x, float y)
{
	// top left is (0,0)
	float delta = 10.0f;
	for (auto model : models)
	{
		auto p = model->GetTranslation();
		Vector4 modelPosition = { p.x,p.y,p.z,1 };
		auto viewMatrix = activeCamera->GetViewMatrix();
		auto perspectiveMatrix = activeCamera->GetPerspectiveMatrix();
		auto modelViewPosition = Vector4::Transform(modelPosition, viewMatrix);
		auto modelPerspectivePosition = Vector4::Transform(modelViewPosition, perspectiveMatrix);
		modelPerspectivePosition /= modelPerspectivePosition.w;
		float modelX = (modelPerspectivePosition.x + 1) / 2.0f * windowSize.cx;
		float modelY = windowSize.cy - (modelPerspectivePosition.y + 1) / 2.0f * windowSize.cy;

		Vector2 modelScreenPos = { modelX, modelY };
		Vector2 clickScreenPos = { x,y };
		auto diff = clickScreenPos - modelScreenPos;
		auto lengthSqrt = diff.LengthSquared();
		if (lengthSqrt < delta * delta)
		{
			ChangeSelection(model->id);
			break;
		}
	}
}
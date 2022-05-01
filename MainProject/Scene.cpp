#include "Scene.h"
#include "Torus.h"
#include "Point.h"
#include "SimpleMath.h"
#include "BezierCurveC0.h"
#include "BezierCurveC2.h"
#include "BezierCurveInterpolating.h"
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


std::shared_ptr<Torus> Scene::AddTorus()
{
	auto torus = std::make_shared<Torus>(5.0f, 2.0f, 20, 20);
	torus->SetTranslation(cursor->translation.x, cursor->translation.y, cursor->translation.z);
	AddModel(torus);
	return torus;
}

std::shared_ptr<Point> Scene::AddPoint()
{
	auto point = std::make_shared<Point>(Vector3(cursor->translation));
	point->onRemovedFromSceneCallback.push_back({ -1,[this](std::shared_ptr<Point> point) {
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
	int i  = model.use_count();
	model->OnAddedToScene();
}

void Scene::AddBezierCurveC0FromSelectedPoints()
{
	auto selectedPointsSh = GetSelectedPoints();
	auto bezier = std::make_shared<BezierCurveC0>(selectedPointsSh);
	AddModel(bezier);
}

void Scene::AddBezierCurveC2FromSelectedPoints()
{
	auto selectedPointsSh = GetSelectedPoints();
	auto bezier = std::make_shared<BezierCurveC2>(selectedPointsSh);
	AddModel(bezier);
}

void Scene::AddBezierCurveInterpolatingFromSelectedPoints()
{
	auto selectedPointsSh = GetSelectedPoints();
	auto bezier = std::make_shared<BezierCurveInterpolating>(selectedPointsSh);
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

void Scene::Select(std::shared_ptr<IModel> model)
{
	for (auto [id, m] : composite->modelsMap)
	{
		m->OnDeselect();
	}
	this->composite = std::make_shared<CompositeModel>();
	this->composite->AddModel(model);
	this->selectedModel = model;
	model->OnSelect();
}

void Scene::ChangeSelection(std::shared_ptr<IModel> model)
{
	const bool is_selected = IsSelcted(model->id);
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

std::shared_ptr<IModel> Scene::GetModelFromScreenCoords(float x, float y)
{
	// top left is (0,0)
	float delta = 10.0f;
	auto viewMatrix = activeCamera->GetViewMatrix();
	auto perspectiveMatrix = activeCamera->GetPerspectiveMatrix();
	auto VP = viewMatrix * perspectiveMatrix;
	for (auto model : models)
	{
		auto modelPtr = model->SelectFromScreenCoords(x, y, VP);
		if (modelPtr)
		{
			return modelPtr;
		}
	}
	return nullptr;
}


void Scene::UpdateCursorPositionFromScreenCoords(Vector2 screenCoords)
{
	screenCoords.y = windowSize.cy - screenCoords.y;
	Matrix viewMatrx = activeCamera->GetViewMatrix();
	Matrix perspectiveMatrix = activeCamera->GetPerspectiveMatrix();
	auto perspectiveMatrixInverted = perspectiveMatrix.Invert();
	auto viewMatrxInverted = viewMatrx.Invert();

	Vector4 currentGlobalPosition(cursor->translation.x, cursor->translation.y, cursor->translation.z, 1);
	auto currentCameraPosition = Vector4::Transform(currentGlobalPosition, viewMatrx);
	auto currentPerspectivePosition = Vector4::Transform(currentCameraPosition, perspectiveMatrix);
	float w = currentPerspectivePosition.w;
	currentPerspectivePosition = currentPerspectivePosition / currentPerspectivePosition.w;

	Vector2 normalizedScreenCoords = {  screenCoords.x * 2.0f / windowSize.cx - 1,screenCoords.y * 2.0f / windowSize.cy - 1 };

	Vector4 newPerspectivePostion = { normalizedScreenCoords.x, normalizedScreenCoords.y, 1 ,1 };
	newPerspectivePostion = newPerspectivePostion * max(w, -w);

	auto newCameraPosition = Vector4::Transform(newPerspectivePostion, perspectiveMatrixInverted);
	newCameraPosition.w = 1;
	auto newWorld = Vector4::Transform(newCameraPosition, viewMatrxInverted);

	cursor->translation = { newWorld.x, newWorld.y, newWorld.z };
}

void Scene::RemoveSelectedModels()
{
	for (auto [id, model] : composite->modelsMap)
	{
		DeleteModel(id);
	}
	selectedModel = nullptr;
	composite = std::make_shared<CompositeModel>();
}

void Scene::DrawScene()
{
	for (auto model : models)
	{
		if (model->GetVisible())
		{
			model->Draw(activeCamera);
		}
	}
	if (selectedModel)
	{
		selectedModel->Draw(activeCamera);
	}
	cursor->Draw(activeCamera);
}

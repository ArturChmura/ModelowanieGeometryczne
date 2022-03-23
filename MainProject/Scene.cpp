#include "Scene.h"
#include "Torus.h"
#include "Point.h"
#include "SimpleMath.h"
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


void Scene::AddModel(std::shared_ptr<IModel> model)
{
	models.push_back(model);
}

void Scene::AddTorus()
{
	auto torus = std::make_shared<Torus>(5, 2, 20, 20);
	torus->SetTranslation(cursor->translation.x, cursor->translation.y, cursor->translation.z);
	AddModel(torus);
}

void Scene::AddPoint()
{
	auto point = std::make_shared<Point>(cursor->translation);
	AddModel(point);
}

void Scene::DeleteModel(int modelId)
{
	auto new_end = std::remove_if(models.begin(), models.end(),
		[modelId](const std::shared_ptr<IModel>& model)
		{ return model->id == modelId; });
	models.erase(new_end, models.end());
}

void Scene::ChangeSelection(int modelId)
{
	const bool is_selected = IsSelcted(modelId);
	auto it = std::find_if(models.begin(), models.end(), [modelId](const auto& m) { return m->id == modelId; });
	auto model = *it;
	if (is_selected)
	{
		composite->RemoveModel(model);
	}
	else
	{
		composite->AddModel(model);
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
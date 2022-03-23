#include "Scene.h"
#include "Torus.h"
#include "Point.h"

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

Scene::Scene(std::shared_ptr<Coursor3d> cursor)
{
	this->cursor = cursor;
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
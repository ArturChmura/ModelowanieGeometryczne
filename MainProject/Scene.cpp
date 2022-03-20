#include "Scene.h"

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

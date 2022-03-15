#include "Scene.h"

void Scene::AddCamera(shared_ptr<Camera> camera)
{
	cameras.push_back(camera);
	if (cameras.size() == 1)
	{
		activeCamera = camera;
	}
}

void Scene::SetActiveCamera(shared_ptr<Camera> camera)
{
	if (std::find(cameras.begin(), cameras.end(), camera) == cameras.end())
	{
		cameras.push_back(camera);
	}
	activeCamera = camera;
}

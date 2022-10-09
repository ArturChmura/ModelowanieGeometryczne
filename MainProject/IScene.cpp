#include "IScene.h"

IScene::IScene(std::string name)
	:Named(name)
{
}

void IScene::AddCamera(std::shared_ptr<Camera> camera)
{
	cameras.push_back(camera);
	if (cameras.size() == 1)
	{
		activeCamera = camera;
	}
}

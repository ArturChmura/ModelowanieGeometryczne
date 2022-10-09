#pragma once
#include "Camera.h"
#include <string>
class IScene : public Named
{
public:
	IScene(std::string name);
	void AddCamera(std::shared_ptr<Camera> camera);
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;

	virtual void DrawScene() = 0;
};
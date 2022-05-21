#pragma once
#include <memory>
#include "Scene.h"

class MessageHandler
{
public:
	MessageHandler(std::shared_ptr<Scene> scene);
	void HandleMessage(MSG message);
	float xWhenDown, yWhenDown;

private:
	std::shared_ptr<Scene> scene;
};
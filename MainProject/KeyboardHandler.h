#pragma once
#include <memory>
#include "Scene.h"

class KeyboardHandler
{
public:
	KeyboardHandler(std::shared_ptr<Scene> scene);
	void HandleKeyboard();

private:
	std::shared_ptr<Scene> scene;
};
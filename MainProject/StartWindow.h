#pragma once
#include "IScene.h"
#include <memory>
#include "IGuiWindow.h"

class StartWindow : public IGuiWindow
{
public:
	StartWindow(
		std::vector<std::shared_ptr<IScene>> scenes, 
		std::function<void(std::shared_ptr<IScene>)> changeSceneLambda);
	void Render();
	std::vector<std::shared_ptr<IScene>> scenes;
	std::function<void(std::shared_ptr<IScene>)> changeSceneLambda;
private:
};
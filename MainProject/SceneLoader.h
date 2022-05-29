#pragma once
#include <memory>
#include "Scene.h"
#include <filesystem>
class SceneLoader
{
public:
	static void LoadScene(std::shared_ptr<Scene> scene, std::filesystem::path path);
};
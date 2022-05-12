#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <d3d11.h>
#include "SimpleMath.h"
#include "ICameraMovement.h"
#include "Named.h"
class Scene;
// Interface
class Camera: public Named
{
public:

	Camera(std::shared_ptr< ICameraMovement> cameraMovement, std::string name);
	virtual DirectX::SimpleMath::Matrix GetViewMatrix() = 0;
	virtual DirectX::SimpleMath::Matrix GetPerspectiveMatrix() = 0;
	std::shared_ptr<ICameraMovement> cameraMovement;
	virtual void RenderScene(std::shared_ptr<Scene> scene) = 0;

	virtual void DrawGUI() = 0;
};
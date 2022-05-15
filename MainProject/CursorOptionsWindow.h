#pragma once
#include "DirectXMath.h"
#include <memory>
#include "Coursor3d.h"
#include "Scene.h"
#include <d3d11.h>
#include "SimpleMath.h"
#include "IGuiWindow.h"

class CursorOptionsWindow : public IGuiWindow
{
public:
	CursorOptionsWindow(std::shared_ptr<Coursor3d> cursor3d, std::shared_ptr<Scene> scene, SIZE windowSize);
	void Render();
	void UpdateScreenCoords();
	std::shared_ptr<Coursor3d> cursor3d;
	std::shared_ptr<Scene> scene;
	SIZE windowSize;
	DirectX::SimpleMath::Vector2 screenCoords;

};
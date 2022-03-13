#pragma once
#include "ColorRGBAfloat.h"
#include <DirectXMath.h>
#include "Application.h"
#include <memory>
class OptionsWindow
{
public:
	OptionsWindow(std::shared_ptr<Application> application);
	void Render();

	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 torusColor;

	std::shared_ptr<Application> application;
};
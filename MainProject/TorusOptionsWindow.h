#pragma once
#include <DirectXMath.h>
#include "Torus.h"
#include <memory>

class TorusOptionsWindow
{
public:
	TorusOptionsWindow(std::shared_ptr<Torus> torus);
	void Render();
	std::shared_ptr<Torus> torus;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotation;
	float R;
	float r;
	int largeSlices;
	int smallSlices;

	DirectX::XMFLOAT3 torusColor;
};
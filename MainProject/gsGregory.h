#pragma once
#include <memory>
#include "dxptr.h"
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"
#include <array>

__declspec(align(16))
struct GSGregoryConstantBuffer
{
	DirectX::SimpleMath::Matrix mvp;

	std::array<DirectX::SimpleMath::Vector3, 4> p;
	std::array<DirectX::SimpleMath::Vector3, 4> dU;
	std::array<DirectX::SimpleMath::Vector3, 4> dV;
	std::array<DirectX::SimpleMath::Vector3, 4> dUV;
	std::array<DirectX::SimpleMath::Vector3, 4> dVU;
	int horizontalSlices;
	int verticalSlices;
};

class GSGregory
{
public:
	GSGregory();

	mini::dx_ptr<ID3D11GeometryShader> m_geometryShader;
	mini::dx_ptr<ID3D11Buffer> constantBufferVS;

	void SetConstantBuffer(GSGregoryConstantBuffer buffer);
};

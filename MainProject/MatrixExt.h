#pragma once
// ReSharper disable once CppUnusedIncludeDirective 
#include <d3d11.h>
#include <d3d11.h>
#include "SimpleMath.h"

class MatrixExt
{
public:
	static DirectX::SimpleMath::Vector3 DecomposeScale(DirectX::SimpleMath::Matrix matrix);
	static DirectX::SimpleMath::Vector4 DecomposeTranslation(DirectX::SimpleMath::Matrix matrix);
	static DirectX::SimpleMath::Vector3 DecomposeRotation(DirectX::SimpleMath::Matrix matrix);
};
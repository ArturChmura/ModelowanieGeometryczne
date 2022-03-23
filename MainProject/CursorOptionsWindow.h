#pragma once
#include "DirectXMath.h"
#include <memory>
#include "Coursor3d.h"
class CursorOptionsWindow
{
public:
	CursorOptionsWindow(std::shared_ptr<Coursor3d> cursor3d);
	void Render();
	DirectX::XMFLOAT3 translation; 
	std::shared_ptr<Coursor3d> cursor3d;

};
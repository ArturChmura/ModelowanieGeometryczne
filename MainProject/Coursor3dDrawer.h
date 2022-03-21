#pragma once
#include "IModelDrawer.h"
#include "Coursor3d.h"

class Coursor3dDrawer : public IModelDrawer
{

public:
	Coursor3dDrawer(
		std::shared_ptr<DxDevice> dxDevice,
		std::shared_ptr<Coursor3d> coursor);

	void Draw(std::shared_ptr<DxDevice> m_device, std::shared_ptr<Camera> camera) override;
	std::shared_ptr<Coursor3d> coursor3d;
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
};
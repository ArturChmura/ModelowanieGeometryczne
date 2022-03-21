#pragma once
#include "IModelDrawer.h"

class GlobalAxisDrawer : public IModelDrawer
{
public:
	GlobalAxisDrawer(std::shared_ptr<DxDevice> dxDevice);
	void Draw(std::shared_ptr<DxDevice> m_device, std::shared_ptr<Camera> camera) override;

	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
};
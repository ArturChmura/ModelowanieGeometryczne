#pragma once
#include "VertexModel.h"
#include "dxptr.h"
#include "dxDevice.h"
#include "Camera.h"
#include "IModelDrawer.h"
class DxVertexModelDrawer : public IModelDrawer
{
public:
	DxVertexModelDrawer(
		std::shared_ptr<DxDevice> dxDevice,
		std::shared_ptr<VertexModel> vertexModel
	);

	void Draw(std::shared_ptr<DxDevice> m_device, std::shared_ptr<Camera> camera) override;

	std::shared_ptr<VertexModel> vertexModel;

	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	mini::dx_ptr<ID3D11Buffer> indexBuffer;
	std::shared_ptr<DxDevice> dxDevicce;

private:
	void UpdateBuffers();
};
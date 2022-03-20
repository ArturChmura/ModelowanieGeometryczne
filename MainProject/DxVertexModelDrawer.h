#pragma once
#include "VertexModel.h"
#include "dxptr.h"
#include "dxDevice.h"
class DxVertexModelDrawer
{
public:
	DxVertexModelDrawer(
		std::shared_ptr<DxDevice> dxDevice,
		std::shared_ptr<VertexModel> vertexModel
	);

	std::shared_ptr<VertexModel> vertexModel;

	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	mini::dx_ptr<ID3D11Buffer> indexBuffer;
	std::shared_ptr<DxDevice> dxDevicce;

private:
	void UpdateBuffers();
};
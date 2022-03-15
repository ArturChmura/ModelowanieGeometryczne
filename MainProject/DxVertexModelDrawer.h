#pragma once
#include "VertexModel.h"
#include "dxptr.h"
#include "dxDevice.h"
using namespace std;
class DxVertexModelDrawer
{
public:
	DxVertexModelDrawer(
		shared_ptr<DxDevice> dxDevice,
		std::shared_ptr<VertexModel> vertexModel
	);

	std::shared_ptr<VertexModel> vertexModel;

	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	mini::dx_ptr<ID3D11Buffer> indexBuffer;
	shared_ptr<DxDevice> dxDevicce;

private:
	void UpdateBuffers();
};
#include "MeshInfo.h"
#include "dxDevice.h"

void MeshInfo::SetUpRender()
{
	DxDevice::instance->context()->IASetPrimitiveTopology(topology);
	DxDevice::instance->context()->IASetIndexBuffer(indexBuffer.get(),
		DXGI_FORMAT_R32_UINT, 0);
}

#include "DxVertexModelDrawer.h"
#include <stdio.h>

DxVertexModelDrawer::DxVertexModelDrawer(shared_ptr<DxDevice> dxDevice, std::shared_ptr<VertexModel> vertexModel)
{
	this->vertexModel = vertexModel;
	this->dxDevicce = dxDevice;
	vertexModel->act.push_back([this]() {UpdateBuffers(); });
	UpdateBuffers();
}

void DxVertexModelDrawer::UpdateBuffers()
{
	vertexBuffer = dxDevicce->CreateVertexBuffer(vertexModel->GetVertices());
	indexBuffer = dxDevicce->CreateVertexBuffer(vertexModel->GetIndices());
}

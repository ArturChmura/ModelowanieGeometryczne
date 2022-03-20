#include "DxVertexModelDrawer.h"
#include <stdio.h>

DxVertexModelDrawer::DxVertexModelDrawer(std::shared_ptr<VertexModel> vertexModel, std::shared_ptr < DxDevice> dxDevice)
{
	this->vertexModel = vertexModel;
	this->dxDevice = dxDevice;
	vertexModel->act.push_back([this]() {UpdateBuffers(); });
	UpdateBuffers();
}

void DxVertexModelDrawer::UpdateBuffers()
{
	vertexBuffer = dxDevice->CreateVertexBuffer(vertexModel->GetVertices());
	indexBuffer = dxDevice->CreateIndexBuffer(vertexModel->GetIndices());
}

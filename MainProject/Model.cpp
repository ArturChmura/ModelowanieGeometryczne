#include "Model.h"
Model::Model()
{
	DirectX::XMStoreFloat4x4(&modelMatrix,DirectX::XMMatrixIdentity());
	this->scale = { 1,1,1 };
	this->translation = { 0,0,0 };
}

void Model::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
	UpdateModelMatrix();
}

void Model::SetTranslation(float x, float y, float z)
{
	translation = { x,y,z };
	UpdateModelMatrix();
}

void Model::UpdateModelMatrix()
{
	DirectX::XMStoreFloat4x4(&modelMatrix,
		DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z)*
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
	);
}

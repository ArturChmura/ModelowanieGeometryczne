#include "Model.h"
Model::Model()
{
	this->scale = { 1,1,1 };
	this->translation = { 0,0,0 };
	this->rotation = { 0,0,0 };
	UpdateModelMatrix();
}

DirectX::XMFLOAT4X4 Model::GetModelMatrix()
{
	return modelMatrix;
}

void Model::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
	UpdateModelMatrix();
}

DirectX::XMFLOAT3 Model::GetScale()
{
	return scale;
}

void Model::SetTranslation(float x, float y, float z)
{
	translation = { x,y,z };
	UpdateModelMatrix();
}

DirectX::XMFLOAT3 Model::GetTranslation()
{
	return translation;
}

void Model::SetRotation(float x, float y, float z)
{
	rotation = { x,y,z };
	UpdateModelMatrix();
}

DirectX::XMFLOAT3 Model::GetRotation()
{
	return rotation;
}

void Model::UpdateModelMatrix()
{
	DirectX::XMStoreFloat4x4(&modelMatrix,
		DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) *
		GetRotationMatrix() *
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
	);
}

DirectX::XMMATRIX Model::GetRotationMatrix()
{
	return DirectX::XMMatrixRotationX(rotation.x) *
		DirectX::XMMatrixRotationY(rotation.y) *
		DirectX::XMMatrixRotationZ(rotation.z);
}



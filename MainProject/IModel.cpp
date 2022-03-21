#include "IModel.h"
using namespace DirectX;
IModel::IModel()
{
	this->scale = { 1,1,1 };
	this->translation = { 0,0,0 };
	this->rotation = { 0,0,0 };
	UpdateModelMatrix();
}

XMFLOAT4X4 IModel::GetModelMatrix()
{
	return modelMatrix;
}

void IModel::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
	UpdateModelMatrix();
}

XMFLOAT3 IModel::GetScale()
{
	return scale;
}

void IModel::SetTranslation(float x, float y, float z)
{
	translation = { x,y,z };
	UpdateModelMatrix();
}

XMFLOAT3 IModel::GetTranslation()
{
	return translation;
}

void IModel::SetRotation(float x, float y, float z)
{
	rotation = { x,y,z };
	UpdateModelMatrix();
}

XMFLOAT3 IModel::GetRotation()
{
	return rotation;
}

void IModel::ScaleFromPoint(XMFLOAT3 globalPoint, XMFLOAT3 scale)
{
	auto modelMatrix = GetModelMatrix();
	auto inverseMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&modelMatrix));
	auto localPoint = XMVector3Transform(XMLoadFloat3(&globalPoint), inverseMatrix);
	auto scaleMatrix =
		XMMatrixTranslationFromVector(-1.0f * localPoint) *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixTranslationFromVector(localPoint);

	XMStoreFloat4x4(&this->modelMatrix, XMLoadFloat4x4(&modelMatrix) * scaleMatrix);
}

void IModel::RotateFromPoint(XMFLOAT3 globalPoint, XMFLOAT3 rotation)
{
	auto modelMatrix = GetModelMatrix();
	auto inverseMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&modelMatrix));
	auto localPoint = XMVector3Transform(XMLoadFloat3(&globalPoint), inverseMatrix);

	auto rotationMatrix =
		XMMatrixTranslationFromVector(-1.0f * localPoint) *
		XMMatrixRotationX(rotation.x) *
		XMMatrixRotationY(rotation.y) *
		XMMatrixRotationZ(rotation.z) *
		XMMatrixTranslationFromVector(localPoint);

	XMStoreFloat4x4(&this->modelMatrix, XMLoadFloat4x4(&modelMatrix) * rotationMatrix);
}

void IModel::UpdateModelMatrix()
{
	XMStoreFloat4x4(&modelMatrix,
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		GetRotationMatrix() *
		XMMatrixTranslation(translation.x, translation.y, translation.z)
	);
}

XMMATRIX IModel::GetRotationMatrix()
{
	return
		XMMatrixRotationX(rotation.x) *
		XMMatrixRotationY(rotation.y) *
		XMMatrixRotationZ(rotation.z);
}



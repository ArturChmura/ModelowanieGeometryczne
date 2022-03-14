#include "ArcCameraModel.h"
#include "Helpers.h"
using namespace DirectX;
DirectX::XMFLOAT4X4 ArcCameraModel::GetViewMatrix()
{
	return viewMatrix;
}
DirectX::XMFLOAT4X4 ArcCameraModel::GetPerspectiveMatrix()
{
	return perspectiveMatrix;
}
void ArcCameraModel::Rotate(float firstAxis, float secondAxis)
{
	firstAxis = -firstAxis;
	secondAxis = -secondAxis;
	DirectX::XMFLOAT4X4 rotationMatrixFirst;
	DirectX::XMStoreFloat4x4(&rotationMatrixFirst, DirectX::XMMatrixRotationAxis(XMLoadFloat3(&upVector), firstAxis));

	auto direction = GetCameraDirection();
	auto dot = XMVector3Dot(direction, XMLoadFloat3(&upVector));
	float dotF;
	XMStoreFloat(&dotF, dot);
	if (dotF * sgn(-secondAxis) > 0.999f)
	{
		secondAxis = 0.0f;
	}

	DirectX::XMFLOAT3 right{ viewMatrix._11, viewMatrix._21 ,viewMatrix._31 };
	DirectX::XMFLOAT4X4 rotationMatrixSecond;
	DirectX::XMStoreFloat4x4(&rotationMatrixSecond, DirectX::XMMatrixRotationAxis(XMLoadFloat3(&right), secondAxis));


	auto vectorSubtracted = DirectX::XMVectorSubtract(
		DirectX::XMLoadFloat3(&position),
		DirectX::XMLoadFloat3(&targerPostion)
	);

	auto transformed = DirectX::XMVector4Transform(vectorSubtracted, DirectX::XMLoadFloat4x4(&rotationMatrixFirst));
	transformed = DirectX::XMVector4Transform(transformed, DirectX::XMLoadFloat4x4(&rotationMatrixSecond));

	auto added = DirectX::XMVectorAdd(transformed, DirectX::XMLoadFloat3(&targerPostion));

	DirectX::XMStoreFloat3(&position, added);

	UpdateViewMatrix();
}

void ArcCameraModel::ChangeDistance(float distanceChange)
{
	auto direction = GetCameraDirection();
	auto length = XMVector3LengthSq(direction);
	float lengthF;
	XMStoreFloat(&lengthF, length);
	if (lengthF == 0)
	{
		direction = { 0,0,1 };
	}

	auto toAdd = -distanceChange * direction;
	auto newPosition = XMVectorAdd(
		XMLoadFloat3(&position),
		toAdd
	);
	XMStoreFloat3(&position, newPosition);

	UpdateViewMatrix();
}

void ArcCameraModel::LookAt(float x, float y, float z)
{
	targerPostion = { x,y,z };
	UpdateViewMatrix();
}

void ArcCameraModel::SetPerspective(float fieldOfViewY, float aspectRatioXperY, float nearZ, float farZ)
{
	DirectX::XMStoreFloat4x4(&perspectiveMatrix, DirectX::XMMatrixPerspectiveFovLH(
		fieldOfViewY,
		aspectRatioXperY,
		nearZ,
		farZ));
}

void ArcCameraModel::UpdateViewMatrix()
{
	auto matrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&position),
		DirectX::XMLoadFloat3(&targerPostion),
		DirectX::XMLoadFloat3(&upVector)
	);


	DirectX::XMStoreFloat4x4(&viewMatrix, matrix);

}

DirectX::XMVECTOR ArcCameraModel::GetCameraDirection()
{
	XMFLOAT3 direction = { targerPostion.x - position.x, targerPostion.y - position.y, targerPostion.z - position.z };
	auto normalized = XMVector3Normalize(XMLoadFloat3(&direction));
	return normalized;
}

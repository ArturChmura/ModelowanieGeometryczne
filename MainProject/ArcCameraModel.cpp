#include "ArcCameraModel.h"
#include "Helpers.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;


ArcCameraModel::ArcCameraModel(DirectX::SimpleMath::Vector3 targetPosition, float distance)
{
	this->targetPosition = targetPosition;
	XMFLOAT3 offset = { 0, 0, 0 };
	XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&targetPosition), XMLoadFloat3(&offset)));
	ChangeDistance(distance);
}

Matrix ArcCameraModel::GetViewMatrix()
{
	return viewMatrix;
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
		DirectX::XMLoadFloat3(&targetPosition)
	);

	auto transformed = DirectX::XMVector4Transform(vectorSubtracted, DirectX::XMLoadFloat4x4(&rotationMatrixFirst));
	transformed = DirectX::XMVector4Transform(transformed, DirectX::XMLoadFloat4x4(&rotationMatrixSecond));

	auto added = DirectX::XMVectorAdd(transformed, DirectX::XMLoadFloat3(&targetPosition));

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
		direction = { 0,0,-1 };
	}

	auto currentDistanceToTargetVector = XMVector3Length(
		XMVectorSubtract(
			XMLoadFloat3(&position),
			XMLoadFloat3(&targetPosition)
		)
	);
	XMFLOAT3 currentDistanceToTarget;
	XMStoreFloat3(&currentDistanceToTarget, currentDistanceToTargetVector);

	if (currentDistanceToTarget.x <= -distanceChange)
	{
		distanceChange = -currentDistanceToTarget.x + 0.001f;
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
	targetPosition = { x,y,z };
	UpdateViewMatrix();
}

void ArcCameraModel::Move(float x, float y, float z)
{
	targetPosition = targetPosition + Vector3(x, y, z);
	UpdateViewMatrix();
}



void ArcCameraModel::UpdateViewMatrix()
{
	auto target = targetPosition;
	if (position == targetPosition)
	{
		target = { position.x , position.y ,position.z + 1 };
	}
	auto matrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&position),
		DirectX::XMLoadFloat3(&target),
		DirectX::XMLoadFloat3(&upVector)
	);

	DirectX::XMStoreFloat4x4(&viewMatrix, matrix);

}

Vector3 ArcCameraModel::GetCameraDirection()
{
	Vector3 direction = { targetPosition.x - position.x, targetPosition.y - position.y, targetPosition.z - position.z };
	direction.Normalize();
	return direction;
}

void ArcCameraModel::DrawGUI()
{
	
}

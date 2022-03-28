#include "PerspectiveCamera.h"
#include "DirectXMath.h"
using namespace DirectX;
PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRatio, float nearZ, float farZ)
{
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;
	UpdatePerspectiveMatrix();
}
void PerspectiveCamera::SetFieldOfView(float fov)
{
	this->fieldOfView = fov;
	UpdatePerspectiveMatrix();
}

void PerspectiveCamera::SetNearZ(float nearZ)
{
	this->nearZ = min(nearZ, this->farZ - 0.001f);
	UpdatePerspectiveMatrix();
}

void PerspectiveCamera::SetFarZ(float farZ)
{
	this->farZ = max(farZ, this->nearZ + 0.001f);
	UpdatePerspectiveMatrix();
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	UpdatePerspectiveMatrix();
}

void PerspectiveCamera::UpdatePerspectiveMatrix()
{
	XMStoreFloat4x4(&perspectiveMatrix,
		XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearZ, farZ)
	);
}

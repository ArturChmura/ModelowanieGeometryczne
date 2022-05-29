#include "PerspectiveCamera.h"
#include "DirectXMath.h"
#include "imgui.h"
#include "dxDevice.h"
#include "Scene.h"
using namespace DirectX;
PerspectiveCamera::PerspectiveCamera(std::shared_ptr<ICameraMovement> cameraMovement, float fieldOfView, float aspectRatio, float nearZ, float farZ, std::string name)
	: Camera(cameraMovement, name)
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


DirectX::SimpleMath::Matrix PerspectiveCamera::GetPerspectiveMatrix()
{
	return perspectiveMatrix;
}

void PerspectiveCamera::UpdatePerspectiveMatrix()
{
	XMStoreFloat4x4(&perspectiveMatrix,
		XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearZ, farZ)
	);
}

DirectX::SimpleMath::Matrix PerspectiveCamera::GetViewMatrix()
{
	return cameraMovement->GetViewMatrix();
}

void PerspectiveCamera::DrawGUI()
{

	if (ImGui::SliderFloat("Field of View", &fieldOfView, 0.1f, 3.1415))
	{
		SetFieldOfView(fieldOfView);
	}
	if (ImGui::SliderFloat("AspectRatio", &aspectRatio, 0.1, 10.0f))
	{
		SetAspectRatio(aspectRatio);
	}
	if (ImGui::SliderFloat("Near Z", &nearZ, 0.01f, farZ))
	{
		SetNearZ(nearZ);
	}
	if (ImGui::SliderFloat("Far Z", &farZ, 1.0f, 1000.0f))
	{
		SetFarZ(farZ);
	}

	cameraMovement->DrawGUI();
}

void PerspectiveCamera::RenderScene(std::shared_ptr<Scene> scene)
{
	int BS_MASK = 0xffffffff;
	DxDevice::instance->context()->OMSetBlendState(nullptr, nullptr, BS_MASK);
	scene->DrawScene();
}

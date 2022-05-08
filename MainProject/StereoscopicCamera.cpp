#include "StereoscopicCamera.h"
#include "dxStructures.h"
#include "dxDevice.h"
#include "Scene.h"
#include "ImGui/imgui.h"

using namespace DirectX::SimpleMath;

StereoscopicCamera::StereoscopicCamera(std::shared_ptr<ICameraMovement> cameraMovement, float fieldOfView, float aspectRatio, float nearZ, float farZ, float eyeDistance, float focusLength)
	:PerspectiveCamera(cameraMovement, fieldOfView, aspectRatio, nearZ, farZ, "Stereoscopic Camera")
{
	this->eyeDistance = eyeDistance;
	this->focusLength = focusLength;
	UpdatePerspectiveMatrix();
}

void StereoscopicCamera::SetEyeDistance(float eyeDistance)
{
	this->eyeDistance = eyeDistance;
	UpdatePerspectiveMatrix();
}

float StereoscopicCamera::GetEyeDistance()
{
	return eyeDistance;
}

void StereoscopicCamera::SetFocusLenght(float focusLenght)
{
	this->focusLength = focusLenght;
	UpdatePerspectiveMatrix();
}

float StereoscopicCamera::GetFocusLenght()
{
	return focusLength;
}

void StereoscopicCamera::SetEye(bool isLeftEye)
{
	this->isLeftEye = isLeftEye;
}

DirectX::SimpleMath::Matrix StereoscopicCamera::GetViewMatrix()
{
	auto normalViewMatrix = cameraMovement->GetViewMatrix();
	float offset = isLeftEye ? -eyeDistance / 2.0f : eyeDistance / 2.0f;
	Matrix viewMatrix = normalViewMatrix * Matrix::CreateTranslation(offset, 0, 0);
	return viewMatrix;
}

DirectX::SimpleMath::Matrix StereoscopicCamera::GetPerspectiveMatrix()
{
	return isLeftEye ? leftPerspectiveMatrix : rightPerspectiveMatrix;
}

void StereoscopicCamera::UpdatePerspectiveMatrix()
{
	float height = tanf(fieldOfView / 2.0f) * focusLength * 2;
	float width = aspectRatio * height;

	//left eye
	{
		float L = -(width - eyeDistance) / 2.0f;
		float l = L * nearZ / focusLength;

		float R = width + L;
		float r = R * nearZ / focusLength;

		leftPerspectiveMatrix = this->CreatePerspectiveMatrix(l, r);
	}

	//right eye
	{
		float R = (width - eyeDistance) / 2.0f;
		float r = R * nearZ / focusLength;

		float L = -(width - R);
		float l = L * nearZ / focusLength;

		rightPerspectiveMatrix = this->CreatePerspectiveMatrix(l, r);
	}
}

void StereoscopicCamera::RenderScene(std::shared_ptr<Scene> scene)
{
	int BS_MASK = 0xffffffff;

	SetEye(true);
	BlendDescription addBsDesc;
	addBsDesc.RenderTarget[0].BlendEnable = true;
	addBsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR;
	auto m_bsAdd = DxDevice::instance->CreateBlendState(addBsDesc);
	DxDevice::instance->context()->OMSetBlendState(m_bsAdd.get(), this->leftEyeColors, BS_MASK);
	scene->DrawScene();

	SetEye(false);
	addBsDesc.RenderTarget[0].BlendEnable = true;
	addBsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR;
	addBsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
	addBsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	m_bsAdd = DxDevice::instance->CreateBlendState(addBsDesc);
	DxDevice::instance->context()->OMSetBlendState(m_bsAdd.get(), this->rightEyeColors, BS_MASK);
	scene->DrawScene();
}

void StereoscopicCamera::DrawGUI()
{
	PerspectiveCamera::DrawGUI();

	float focusLength = GetFocusLenght();
	if (ImGui::SliderFloat("Focus Length", &focusLength, nearZ, farZ))
	{
		SetFocusLenght(focusLength);
	}
	float eyeDistance = GetEyeDistance();
	if (ImGui::SliderFloat("Eye Distance", &eyeDistance, 0.0, 50.0f))
	{
		SetEyeDistance(eyeDistance);
	}

	if (ImGui::ColorPicker3("Left Eye Color", leftEyeColors, ImGuiColorEditFlags_::ImGuiColorEditFlags_DisplayRGB));
	if (ImGui::ColorPicker3("Right Eye Color", rightEyeColors, ImGuiColorEditFlags_::ImGuiColorEditFlags_DisplayRGB));

}

DirectX::SimpleMath::Matrix StereoscopicCamera::CreatePerspectiveMatrix(float l, float r)
{
	auto matrix = Matrix(
		2 * nearZ / (r - l), 0, (r + l) / (r - l), 0,
		0, 1 / tanf(fieldOfView / 2), 0, 0,
		0, 0, (farZ + nearZ) / (farZ - nearZ), (-2 * farZ * nearZ) / (farZ - nearZ),
		0, 0, 1, 0
	);

	return matrix.Transpose();
}


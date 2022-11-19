#include "OrthographicCamera.h"
#include "DirectXMath.h"
using namespace DirectX;

OrthographicCamera::OrthographicCamera(std::shared_ptr<ICameraMovement> cameraMovement, float viewWidth, float viewHeight, float nearZ, float farZ, std::string name)
	: Camera(cameraMovement, name)
{
	this->viewWidth = viewWidth;
	this->viewHeight = viewHeight;

	XMStoreFloat4x4(&perspectiveMatrix,
		XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ)
	);
}

DirectX::SimpleMath::Matrix OrthographicCamera::GetPerspectiveMatrix()
{
	return perspectiveMatrix;
}

DirectX::SimpleMath::Matrix OrthographicCamera::GetViewMatrix()
{
	return cameraMovement->GetViewMatrix();
}

void OrthographicCamera::DrawGUI()
{
}

void OrthographicCamera::RenderScene(std::shared_ptr<IScene> scene)
{
}

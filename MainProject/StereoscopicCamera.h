#pragma once
#include "PerspectiveCamera.h"
#include "dxptr.h"

class StereoscopicCamera : public PerspectiveCamera
{
public:
	StereoscopicCamera(std::shared_ptr<ICameraMovement> cameraMovement, float fieldOfView, float aspectRatio, float nearZ, float farZ, float eyeDistance, float focusLength, std::function<ID3D11DepthStencilView*()> depthBufferGetter);

	void SetEyeDistance(float eyeDistance);
	float GetEyeDistance();

	void SetFocusLenght(float focusLenght);
	float GetFocusLenght();
	void SetEye(bool left);
	virtual DirectX::SimpleMath::Matrix GetViewMatrix() override;
	virtual DirectX::SimpleMath::Matrix GetPerspectiveMatrix() override;
	virtual void UpdatePerspectiveMatrix() override;
	virtual void RenderScene(std::shared_ptr<Scene> scene) override;
	virtual void DrawGUI() override;
	std::function<ID3D11DepthStencilView*()> depthBufferGetter;
private:
	bool isLeftEye = true;
	float eyeDistance;
	float focusLength;

	DirectX::SimpleMath::Matrix leftPerspectiveMatrix;
	DirectX::SimpleMath::Matrix rightPerspectiveMatrix;
	float leftEyeColors[4] = { 0.0f,1.0f,1.0f,1.0f };
	float rightEyeColors[4] =  { 1.0f, 0.0f, 0.0f, 1.0f };

	mini::dx_ptr<ID3D11BlendState> m_bsAdd;
};
#include "SingleGregoryPatch.h"
#include <stdexcept>
#include "ShadersManager.h"

using namespace DirectX::SimpleMath;





SingleGregoryPatch::SingleGregoryPatch(std::array<DirectX::SimpleMath::Vector3, 4> p, std::array<DirectX::SimpleMath::Vector3, 4> dU, std::array<DirectX::SimpleMath::Vector3, 4> dV, std::array<DirectX::SimpleMath::Vector3, 4> dUV, std::array<DirectX::SimpleMath::Vector3, 4> dVU)
	:IUnmovableModel("Single Gregory Patch")
{
	this->p = p;
	this->dU = dU;
	this->dV = dV;
	this->dUV = dUV;
	this->dVU = dVU;

	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}

void SingleGregoryPatch::Draw(std::shared_ptr<Camera> camera)
{
	meshInfo.SetUpRender();

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsGregory->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsGregory->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(nullptr, nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsSurfaceBezier->SetVertexBuffer(meshInfo.vertexBuffer.get());
	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	auto v = camera->GetViewMatrix();
	auto p = camera->GetPerspectiveMatrix();
	auto mvp = v * p;

	VSGregoryConstantBuffer gsCB;
	gsCB.mvp = mvp;
	gsCB.p = this->p;
	gsCB.dU = dU;
	gsCB.dV = dV;
	gsCB.dUV = dUV;
	gsCB.dVU = dVU;

	ShadersManager::vsGregory->SetConstantBuffer(gsCB);

	DxDevice::instance->context()->Draw((verticalSlices + 1)*(horizontalSlices+1), 0);

}
void SingleGregoryPatch::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<SingleGregoryPatch>());
}

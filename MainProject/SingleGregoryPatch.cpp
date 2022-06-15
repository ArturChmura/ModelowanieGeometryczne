#include "SingleGregoryPatch.h"
#include <stdexcept>
#include "ShadersManager.h"

using namespace DirectX::SimpleMath;



SingleGregoryPatch::SingleGregoryPatch(
	std::array<DirectX::SimpleMath::Vector3, 4> p, 
	std::array<DirectX::SimpleMath::Vector3, 4> dU, 
	std::array<DirectX::SimpleMath::Vector3, 4> dV, 
	std::array<DirectX::SimpleMath::Vector3, 4> dUV, 
	std::array<DirectX::SimpleMath::Vector3, 4> dVU
)
	:IUnmovableModel("Single Gregory Patch")
{
	this->p = p;
	this->dU = dU;
	this->dV = dV;
	this->dUV = dUV;
	this->dVU = dVU;

	meshInfo.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
	UpdateSlices(20, 20);
}

void SingleGregoryPatch::UpdateSlices(int horizontalSlices, int verticalSlices)
{
	this->horizontalSlices = horizontalSlices;
	this->verticalSlices = verticalSlices;
	std::vector<VSGregoryIn> vertices = std::vector<VSGregoryIn>();

	float stepVertical = 1.0f / verticalSlices;
	for (float tV = 0; tV <= 1.0f + stepVertical / 2; tV += stepVertical)
	{
		VSGregoryIn input;
		input.pos = Vector4(tV, 0, 0, 0);
		vertices.push_back(input);
	}

	float stepHorizontal = 1.0f / horizontalSlices;
	for (float tH = 0; tH <= 1.0f + stepHorizontal / 2; tH += stepHorizontal)
	{
		VSGregoryIn input;
		input.pos = Vector4(tH, 1, 0, 0);
		vertices.push_back(input);
	}


	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
}

void SingleGregoryPatch::UpdatePoints(std::array<DirectX::SimpleMath::Vector3, 4> p, std::array<DirectX::SimpleMath::Vector3, 4> dU, std::array<DirectX::SimpleMath::Vector3, 4> dV, std::array<DirectX::SimpleMath::Vector3, 4> dUV, std::array<DirectX::SimpleMath::Vector3, 4> dVU)
{
	this->p = p;
	this->dU = dU;
	this->dV = dV;
	this->dUV = dUV;
	this->dVU = dVU;
}


void SingleGregoryPatch::Draw(std::shared_ptr<Camera> camera)
{
	meshInfo.SetUpRender();

	DxDevice::instance->context()->IASetInputLayout(ShadersManager::vsGregory->m_layout.get());
	DxDevice::instance->context()->VSSetShader(ShadersManager::vsGregory->m_vertexShader.get(), nullptr, 0);
	DxDevice::instance->context()->GSSetShader(ShadersManager::gsGregory->m_geometryShader.get(), nullptr, 0);
	DxDevice::instance->context()->PSSetShader(ShadersManager::psConstColor->m_pixelShader.get(), nullptr, 0);

	ShadersManager::vsSurfaceBezier->SetVertexBuffer(meshInfo.vertexBuffer.get());
	ShadersManager::psConstColor->SetConstantBuffer(meshInfo.color);

	auto v = camera->GetViewMatrix();
	auto perspectiveMatrix = camera->GetPerspectiveMatrix();
	auto mvp = v * perspectiveMatrix;

	GSGregoryConstantBuffer gsCB;
	gsCB.mvp = mvp;
	gsCB.p = p;
	gsCB.dU = dU;
	gsCB.dV = dV;
	gsCB.dUV = dUV;
	gsCB.dVU = dVU;
	gsCB.horizontalSlices = horizontalSlices;
	gsCB.verticalSlices = verticalSlices;

	ShadersManager::gsGregory->SetConstantBuffer(gsCB);

	DxDevice::instance->context()->Draw(verticalSlices +1 + horizontalSlices + 1, 0);

	if (drawPolygonChain)
	{
		
			Vector3 dUd[4]
			{
				dU[0],
				dU[1],
				-dU[2],
				-dU[3]
			};
			Vector3 dVd[4]
			{
				dV[0],
				-dV[1],
				dV[2],
				-dV[3]
			};
			Vector3 pdU[4];
			Vector3 pdV[4];
			Vector3 pdVdUdVU[4];
			Vector3 pdUdVdUV[4];
			for (int i = 0; i < 4; i++)
			{
				pdU[i] = p[i] + dUd[i] / 3;
				pdV[i] = p[i] + dVd[i] / 3;

				pdVdUdVU[i] = p[i] + dUd[i] / 3 + dVd[i] / 3 + dVU[i] / 9;
				pdUdVdUV[i] = p[i] + dUd[i] / 3 + dVd[i] / 3 + dUV[i] / 9;
			}

			std::vector<Vector3> points
			{
				p[0], pdU[0], pdU[2], p[2], pdV[2], pdV[3], p[3], pdU[3], pdU[1], p[1], pdV[1], pdV[0], p[0]
			};
			PolygonalChain::Draw(camera, points);

			for (int i = 0; i < 4; i++)
			{
				points = 
				{
					pdU[i], pdUdVdUV[i]
				};
				PolygonalChain::Draw(camera, points);
				points =
				{
					pdV[i], pdVdUdVU[i]
				};
				PolygonalChain::Draw(camera, points);
			}
	}

}
void SingleGregoryPatch::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<SingleGregoryPatch>());
}

void SingleGregoryPatch::ChangeColor(DirectX::SimpleMath::Vector3 color)
{
	this->meshInfo.color = color;
}

void SingleGregoryPatch::SetDrawPolygonChain(bool drawPolygonChain)
{
	this->drawPolygonChain = drawPolygonChain;
}
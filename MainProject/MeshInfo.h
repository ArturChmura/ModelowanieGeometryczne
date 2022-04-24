#pragma once
#include "dxptr.h"
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include <SimpleMath.h>

class MeshInfo
{
public:
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	mini::dx_ptr<ID3D11Buffer> indexBuffer;
	DirectX::SimpleMath::Vector3 color = {1,1,1};
	D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	void SetUpRender();
};
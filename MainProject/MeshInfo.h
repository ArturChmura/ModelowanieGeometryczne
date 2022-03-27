#pragma once
#include "dxptr.h"
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
class MeshInfo
{
public:
	mini::dx_ptr<ID3D11Buffer> vertexBuffer;
	mini::dx_ptr<ID3D11Buffer> indexBuffer;
	DirectX::XMFLOAT3 color = {1,1,1};
	D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	void SetUpRender();
};
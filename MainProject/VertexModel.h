#pragma once
#include "IModel.h"
#include <vector>
#include "Vertex.h"
#include <d3d11.h>
#include <memory>
#include <stdio.h>
#include <functional>

class VertexModel :public IModel
{
public:
	size_t GetVerticesCount() { return vertices.size(); };
	size_t GetIndicesCount() { return indices.size(); };

	std::vector<Vertex> GetVertices() { return vertices; };
	std::vector<int> GetIndices() { return indices; }

	std::vector<std::function<void()>> act;
	DirectX::XMFLOAT3 color;
protected:
	std::vector<Vertex> vertices;
	std::vector<int> indices;
};
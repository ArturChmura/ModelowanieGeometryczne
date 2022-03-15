#pragma once
#include "Model.h"
#include <vector>
#include "Vertex.h"
#include <d3d11.h>
#include <memory>
#include <stdio.h>
#include <functional>
using namespace std;

class VertexModel :public Model
{
public:
	size_t GetVerticesCount() { return vertices.size(); };
	size_t GetIndicesCount() { return indices.size(); };

	vector<Vertex> GetVertices() { return vertices; };
	vector<int> GetIndices() { return indices; }

	std::vector<std::function<void()>> act;
protected:
	vector<Vertex> vertices;
	vector<int> indices;
};
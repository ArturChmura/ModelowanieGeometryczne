#pragma once
#include "Model.h"
#include <vector>
#include "Vertex.h"
#include <d3d11.h>
#include <memory>
using namespace std;
class VertexModel :public Model
{
public:
	size_t GetVerticesCount() { return vertices.size(); };
	size_t GetIndicesCount() { return vertices.size(); };

	vector<Vertex> GetVertices() { return vertices; };
	vector<int> GetIndices() { return indices; }
protected:
	vector<Vertex> vertices;
	vector<int> indices;
};
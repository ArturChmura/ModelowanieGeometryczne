#include "Torus.h"
#include "Helpers.h"


Torus::Torus(float R, float r, unsigned int largeSlices, unsigned int smallSlices)
{
	this->R = R;
	this->r = r;
	this->largeSlices = largeSlices;
	this->smallSlices = smallSlices;
	this->color = { 1,0,0 };
	UpdateVertices();
}

void Torus::SetBigRadius(float R)
{
	this->R = R;
	UpdateVertices();
}

void Torus::SetSmallRadius(float r)
{
	this->r = r;
	UpdateVertices();
}

void Torus::SetLargeSlices(int ls)
{
	this->largeSlices = ls;
	UpdateVertices();
}

void Torus::SetSmallSlices(int ss)
{
	this->smallSlices = ss;
	UpdateVertices();
}

void Torus::SetColor(DirectX::XMFLOAT3 color)
{
	this->color = color;
	UpdateVertices();
}

void Torus::UpdateVertices()
{
	auto verticesCount = largeSlices * smallSlices;
	auto indicesCount = largeSlices * smallSlices * 4;

	std::vector<Vertex> verices = std::vector<Vertex>();
	vertices.reserve(verticesCount);
	std::vector<int> indices = std::vector<int>();
	indices.reserve(indicesCount);

	for (int smallCount = 0; smallCount < smallSlices; smallCount++)
	{
		for (int largeCount = 0; largeCount < largeSlices; largeCount++)
		{
			float alpha = 2 * PI * largeCount / largeSlices;
			float beta = 2 * PI * smallCount / smallSlices;
			float x = (R + r * cosf(beta)) * cosf(alpha);
			float y = (R + r * cosf(beta)) * sinf(alpha);
			float z = r * sinf(beta);
			Vertex vertex = { {x,y,z} };
			verices.push_back(vertex);

			Pair<int> topLeft = { largeCount, smallCount };
			Pair<int> topRight = { modulo2(largeCount + 1, largeSlices), smallCount };
			Pair<int> bottomLeft = { largeCount, modulo2(smallCount - 1, smallSlices) };
			Pair<int> bottomRight = { modulo2(largeCount + 1, largeSlices), modulo2(smallCount - 1, smallSlices) };

			// Triangulacja
			/*indices.push_back(topLeft.a + topLeft.b * largeSlices);
			indices.push_back(topRight.a + topRight.b * largeSlices);
			indices.push_back(bottomLeft.a + bottomLeft.b * largeSlices);

			indices.push_back(topRight.a + topRight.b * largeSlices);
			indices.push_back(bottomRight.a + bottomRight.b * largeSlices);
			indices.push_back(bottomLeft.a + bottomLeft.b * largeSlices);*/

			// Siatka
			indices.push_back(topLeft.a + topLeft.b * largeSlices);
			indices.push_back(topRight.a + topRight.b * largeSlices);
			indices.push_back(topLeft.a + topLeft.b * largeSlices);
			indices.push_back(bottomLeft.a + bottomLeft.b * largeSlices);

		}
	}
	this->vertices = verices;
	this->indices = indices;

	for (auto& f : act) { f(); }
}



#include "Torus.h"
#include "Helpers.h"
#include "ImGui/imgui.h"


Torus::Torus(float R, float r, unsigned int largeSlices, unsigned int smallSlices)
	: VertexModel("Torus")
{
	this->R = R;
	this->r = r;
	this->largeSlices = largeSlices;
	this->smallSlices = smallSlices;
	ResetDrawing();
}

void Torus::SetBigRadius(float R)
{
	this->R = R;
	ResetDrawing();
}

void Torus::SetSmallRadius(float r)
{
	this->r = r;
	ResetDrawing();
}

void Torus::SetLargeSlices(int ls)
{
	this->largeSlices = ls;
	ResetDrawing();
}

void Torus::SetSmallSlices(int ss)
{
	this->smallSlices = ss;
	ResetDrawing();
}



void Torus::ResetDrawing()
{
	auto verticesCount = largeSlices * smallSlices;
	auto indicesCount = largeSlices * smallSlices * 4;

	std::vector<Vertex> vertices = std::vector<Vertex>();
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
			vertices.push_back(vertex);

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
	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	this->verticesCount = vertices.size();

	this->meshInfo.indexBuffer = DxDevice::instance->CreateVertexBuffer(indices);
	this->indicesCount = indices.size();
}



void Torus::RenderGUI()
{
	VertexModel::RenderGUI();
	ImGui::Text("Torus size");
	float minSize = 0.1f;
	if (ImGui::DragFloat("R", &R, 0.2f, minSize))
	{
		R = max(R, minSize);
		r = min(r, R);
		ResetDrawing();
	}
	if (ImGui::DragFloat("r", &r, 0.2f, minSize, R))
	{
		r = max(r, minSize);
		r = min(r, R);
		ResetDrawing();
	}

	ImGui::Text("Torus slices");
	if (ImGui::DragInt("Large", &largeSlices, 1, 2))
	{
		largeSlices = max(2, largeSlices);
		ResetDrawing();
	}
	if (ImGui::DragInt("Small", &smallSlices, 1, 2))
	{
		smallSlices = max(2, smallSlices);
		ResetDrawing();
	}
}
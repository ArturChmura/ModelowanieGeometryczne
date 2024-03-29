#include "Torus.h"
#include "Helpers.h"
#include "imgui.h"
#include "vsConstColor.h"
#include "Pair.h"

using namespace DirectX::SimpleMath;

Torus::Torus(float R, float r, unsigned int largeSlices, unsigned int smallSlices)
	: VertexModel("Torus")
{
	this->R = R;
	this->r = r;
	this->largeSlices = largeSlices;
	this->smallSlices = smallSlices;
	UpdateSlicesCount();
}

void Torus::SetBigRadius(float R)
{
	this->R = R;
	UpdateSlicesCount();
}

void Torus::SetSmallRadius(float r)
{
	this->r = r;
	UpdateSlicesCount();
}

void Torus::SetLargeSlices(int ls)
{
	this->largeSlices = ls;
	UpdateSlicesCount();
}

void Torus::SetSmallSlices(int ss)
{
	this->smallSlices = ss;
	UpdateSlicesCount();
}



void Torus::UpdateSlicesCount()
{
	auto verticesCount = largeSlices * smallSlices;
	auto indicesCount = largeSlices * smallSlices * 4;

	std::vector<VSConstColorIn> vertices = std::vector<VSConstColorIn>();
	vertices.reserve(verticesCount);
	std::vector<int> indices = std::vector<int>();
	indices.reserve(indicesCount);

	int** indexArray = new int* [smallSlices];
	for (int i = 0; i < smallSlices; i++)
	{
		indexArray[i] = new int[largeSlices];
		std::fill_n(indexArray[i], largeSlices, -1);
	}
	int index = 0;
	for (int smallCount = 0; smallCount < smallSlices; smallCount++)
	{
		for (int largeCount = 0; largeCount < largeSlices; largeCount++)
		{
			float v = largeCount / (float)largeSlices;
			float u = smallCount / (float)smallSlices;

			if (!IsVisible(u, v))
			{
				continue;
			}
			float alpha = 2 * PI * v;
			float beta = 2 * PI * u;
			float x = (R + r * cosf(beta)) * cosf(alpha);
			float z = (R + r * cosf(beta)) * sinf(alpha);
			float y = r * sinf(beta);
			VSConstColorIn vertex = { Vector3(x,y,z)};
			vertices.push_back(vertex);
			indexArray[smallCount][largeCount] = index++;

			
			// Triangulacja
			/*indices.push_back(topLeft.a + topLeft.b * largeSlices);
			indices.push_back(topRight.a + topRight.b * largeSlices);
			indices.push_back(bottomLeft.a + bottomLeft.b * largeSlices);

			indices.push_back(topRight.a + topRight.b * largeSlices);
			indices.push_back(bottomRight.a + bottomRight.b * largeSlices);
			indices.push_back(bottomLeft.a + bottomLeft.b * largeSlices);*/


		}
	}	
	for (int smallCount = 0; smallCount < smallSlices; smallCount++)
	{
		for (int largeCount = 0; largeCount < largeSlices; largeCount++)
		{
			Pair<int> topLeft = { largeCount, smallCount };
			Pair<int> topRight = { modulo2(largeCount + 1, largeSlices), smallCount };
			Pair<int> bottomLeft = { largeCount, modulo2(smallCount - 1, smallSlices) };
			Pair<int> bottomRight = { modulo2(largeCount + 1, largeSlices), modulo2(smallCount - 1, smallSlices) };
			
			if (indexArray[topLeft.b][topLeft.a] != -1 && indexArray[topRight.b][topRight.a] != -1)
			{
				indices.push_back(indexArray[topLeft.b][topLeft.a]);
				indices.push_back(indexArray[topRight.b][topRight.a]);
			}
			if (indexArray[topLeft.b][topLeft.a] != -1 && indexArray[bottomLeft.b][bottomLeft.a] != -1)
			{
				indices.push_back(indexArray[topLeft.b][topLeft.a]);
				indices.push_back(indexArray[bottomLeft.b][bottomLeft.a]);
			}

		}
	}
	// Siatka

	free(indexArray);
	this->meshInfo.vertexBuffer = DxDevice::instance->CreateVertexBuffer(vertices);
	this->verticesCount = vertices.size();

	this->meshInfo.indexBuffer = DxDevice::instance->CreateIndexBuffer(indices);
	this->indicesCount = indices.size();
}

void Torus::Accept(AbstractModelVisitor& visitor)
{
	visitor.Accept(IModel::downcasted_shared_from_this<Torus>());
}

void Torus::OnFilterUpdate()
{
	UpdateSlicesCount();
}


DirectX::SimpleMath::Vector3 Torus::GetValue(double u, double v)
{
	double alpha = 2 * PI * v;
	double beta = 2 * PI * u;
	double x = (R + r * cosf(2 * PI * u)) * cosf(2 * PI * v);
	double y = r * sinf(2 * PI * u);
	double z = (R + r * cosf(2 * PI * u)) * sinf(2 * PI * v);
	auto localPosition = Vector4(x, y, z, 1);
	auto globalPosition = Vector4::Transform(localPosition, modelMatrix);
	return Vector3(globalPosition);
}

DirectX::SimpleMath::Vector3 Torus::GetUDerivativeValue(double u, double v)
{
	double x = -1 * r * cosf(2 * PI * v) * sinf(2 * PI * u) * 2 * PI;
	double y = 2 * PI * r * cosf(2 * PI * u);
	double z = -2 *PI *r *sinf(2 *PI *u)* sin(2 *PI *v);
	auto localVector = Vector4(x, y, z, 0);
	auto globalPosition = Vector4::Transform(localVector, modelMatrix);
	return Vector3(globalPosition);
}

DirectX::SimpleMath::Vector3 Torus::GetVDerivativeValue(double u, double v)
{
	double x = -1 * R * sinf(2 * PI * v) * 2 * PI + -1 * r * cosf(2 * PI * u) * sinf(2 * PI * v) * 2 * PI;
	double y =  0;
	double z = 2* PI*(R + r* cosf(2 *PI *u)) *cosf(2 *PI* v);
	auto localVector = Vector4(x, y, z, 0);
	auto globalPosition = Vector4::Transform(localVector, modelMatrix);
	return Vector3(globalPosition);
}

bool Torus::IsUWrapped()
{
	return true;
}

bool Torus::IsVWrapped()
{
	return true;
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
		UpdateSlicesCount();
	}
	if (ImGui::DragFloat("r", &r, 0.2f, minSize, R))
	{
		r = max(r, minSize);
		r = min(r, R);
		UpdateSlicesCount();
	}

	ImGui::Text("Torus slices");
	if (ImGui::DragInt("Large", &largeSlices, 1, 2))
	{
		largeSlices = max(2, largeSlices);
		UpdateSlicesCount();
	}
	if (ImGui::DragInt("Small", &smallSlices, 1, 2))
	{
		smallSlices = max(2, smallSlices);
		UpdateSlicesCount();
	}
}
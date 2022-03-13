#include "Cube.h"

std::vector<DirectX::XMFLOAT3> Cube::GetVertices()
{
	return {
		//Front Face
		{ -0.5f, -0.5f, -0.5f },
		{ +0.5f, -0.5f, -0.5f },
		{ +0.5f, +0.5f, -0.5f },
		{ -0.5f, +0.5f, -0.5f },


		//Left Face
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, +0.5f },
		{ -0.5f, +0.5f, +0.5f },
		{ -0.5f, +0.5f, -0.5f },


		//Back Face
		{ -0.5f, -0.5f, +0.5f },
		{ +0.5f, -0.5f, +0.5f },
		{ +0.5f, +0.5f, +0.5f },
		{ -0.5f, +0.5f, +0.5f },

		//Right Face
		{ +0.5f, -0.5f, -0.5f },
		{ +0.5f, -0.5f, +0.5f },
		{ +0.5f, +0.5f, +0.5f },
		{ +0.5f, +0.5f, -0.5f },

		//Top Face
		{ -0.5f, +0.5f, -0.5f },
		{ +0.5f, +0.5f, -0.5f },
		{ +0.5f, +0.5f, +0.5f },
		{ -0.5f, +0.5f, +0.5f },

		//Bottom Face
		{ -0.5f, -0.5f, -0.5f },
		{ +0.5f, -0.5f, -0.5f },
		{ +0.5f, -0.5f, +0.5f },
		{ -0.5f, -0.5f, +0.5f },
	};
}

std::vector<unsigned int> Cube::GetIndices()
{
	return {
	0,2,1, 0,3,2, //front
	4,5,6, 4,6,7, //left
	8,9,10, 8,10,11, //back
	12,14,13, 12,15,14, //right
	16,18,17, 16,19,18, //up
	20,21,22, 20,22,23 //bottom
	};
}

int Cube::GetSize()
{
	return 36;
}

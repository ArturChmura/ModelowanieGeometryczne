#pragma once
#include <vector>
#include "Vertex.h"

struct Triangulation
{
	std::vector<Vertex> Vertices;
	std::vector<int> Indices;
};
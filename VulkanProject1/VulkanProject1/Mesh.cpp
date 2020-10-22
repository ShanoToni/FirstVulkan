#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> verts)
{
	for (auto vertex : verts)
	{
		vertices.push_back(vertex);
	}
}

Mesh::~Mesh()
{
}

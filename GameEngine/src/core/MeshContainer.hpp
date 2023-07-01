#pragma once
#include "Mesh.hpp"


struct MeshContainer
{
public:
	Mesh* mesh;

	std::string* materialName;
	int* materialIndex;

	MeshContainer()
	{
		mesh = new Mesh();
		materialName = new std::string();
		materialIndex = new int();
	}

	~MeshContainer()
	{
		delete mesh;
		delete materialName;
		delete materialIndex;
	}
};
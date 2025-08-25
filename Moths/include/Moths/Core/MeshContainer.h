#pragma once

#include <string>

#include <Moths/API.h>
#include <Moths/Core/Mesh.h>

namespace MothsEngine
{
	struct MOTHS_API MeshContainer
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
}
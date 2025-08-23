#pragma once

#include <vector>
#include <glad/glad.h>

#include "animation/Armature.hpp"
#include "Submesh.hpp"

struct Mesh
{
private:
	std::vector<Submesh*> _submeshes;
	std::vector<std::string> _materials;

public:

	Mesh()
	{
	}

	~Mesh()
	{
		for (auto m : _submeshes)
		{
			delete m;
		}
	}

	void AddSubmesh(Submesh* submesh)
	{
		_submeshes.push_back(submesh);
	}

	void AddMaterialName(std::string materialName)
	{
		_materials.push_back(materialName);
	}

	std::vector<Submesh*> GetSubmeshes()
	{
		return _submeshes;
	}

	std::vector<std::string> GetMaterials()
	{
		return _materials;
	}
};
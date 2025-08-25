#pragma once

#include <vector>
#include <glad/glad.h>

#include <string>

#include <Moths/API.h>
#include <Moths/Animation/Armature.h>
#include <Moths/Core/Submesh.h>
#include <Moths/Core/Material.h>

namespace MothsEngine
{

	struct MOTHS_API Mesh
	{
	private:
		std::vector<Submesh*> _submeshes;
		std::vector<Material> _materials;

	public:

		Mesh()
		{}

		~Mesh()
		{
			for (auto m : _submeshes)
			{
				delete m;
			}
		}

		inline void AddSubmesh(Submesh* submesh)
		{
			_submeshes.push_back(submesh);
		}

		inline void AddMaterial(Material material)
		{
			_materials.push_back(material);
		}

		inline std::vector<Submesh*>& GetSubmeshes()
		{
			return _submeshes;
		}

		inline std::vector<Material>& GetMaterials()
		{
			return _materials;
		}
	};
}
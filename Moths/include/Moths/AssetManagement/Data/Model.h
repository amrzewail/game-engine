#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include <Moths/API.h>
#include <Moths/Core/Vector.h>
#include <Moths/Core/Matrix4x4.h>
#include <Moths/Core/Mesh.h>
#include <Moths/Core/Material.h>

#include <Moths/Animation/Bone.h>
#include <Moths/Animation/Animation.h>
#include <Moths/Animation/Armature.h>


namespace MothsEngine
{
	/// <summary>
	/// - Model
	///		- Meshes
	///			- Submeshes
	///				- Vertices
	///			- Materials
	///		
	///
	/// </summary>

	struct MOTHS_API Model
	{
	private:

	public:
		std::vector<Mesh*> meshes;
		std::map<std::string, Material*> materials;
		std::vector<Animation*> animations;
		std::unordered_map<int, BoneWeight> bones;
		Armature* armature;
		Matrix4x4 globalInverseTransform;

		Model()
		{
			armature = new Armature();
		}

		~Model()
		{
			delete armature;

			for (auto m : meshes)
			{
				delete m;
			}

			for (auto m : materials)
			{
				delete m.second;
			}

			meshes.clear();
		}

	};
}
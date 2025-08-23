#pragma once

#include <vector>
#include "../../Vector.h"
#include <unordered_map>

#include "../../Vector2.h"
#include "../../Matrix4x4.hpp"
#include "../../Mesh.hpp"
#include "../../animation//Bone.h"
#include "../../animation/Animation.hpp"
#include "../../animation/Armature.hpp"

/// <summary>
/// - Model
///		- Meshes
///			- Submeshes
///				- Vertices
///			- Materials
///		
///
/// </summary>

struct Model
{
private:

public:
	std::vector<Mesh*> meshes;
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

		meshes.clear();
	}

};

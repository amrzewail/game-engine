#pragma once
#include <vector>
#include <unordered_map>
#include "animation/BoneWeight.hpp"
#include "animation/MeshBone.hpp"

struct Submesh
{

public:

	std::vector<Vector> vertices;
	std::vector<Vector> normals;
	std::vector<Vector2> uvs;
	std::vector<unsigned int> indices;
	std::vector<BoneWeight> boneWeights;
	std::vector<MeshBone> bones;

	int meshVertexIndex;
	int meshIndiceIndex;
	int materialIndex;


	Submesh()
	{
	}

};
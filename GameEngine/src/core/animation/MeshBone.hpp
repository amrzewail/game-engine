#pragma once
#include "../Matrix4x4.hpp"

struct MeshBone
{
public:
	std::string name;
	Matrix4x4 inverseBindMatrix;

	MeshBone& operator =(const MeshBone rhs)
	{
		this->inverseBindMatrix = rhs.inverseBindMatrix;
		this->name = rhs.name;
		return *this;
	}
};


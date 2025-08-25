#pragma once

#include <Moths/API.h>
#include <Moths/Core/Matrix4x4.h>

namespace MothsEngine
{
	struct MOTHS_API MeshBone
	{
	public:
		std::string name;
		Matrix4x4 inverseBindMatrix;

		inline MeshBone& operator =(const MeshBone rhs)
		{
			this->inverseBindMatrix = rhs.inverseBindMatrix;
			this->name = rhs.name;
			return *this;
		}
	};
}
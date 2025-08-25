#pragma once

#include <iostream>

#include <Moths/API.h>
#include <Moths/Core/Matrix4x4.h>

namespace MothsEngine
{
	struct MOTHS_API Bone
	{

	public:
		int index;
		int parentIndex = -1;
		std::string name;

		Matrix4x4 localTransform;
		Matrix4x4 globalTransform;

		Bone()
		{
			index = 0;
			parentIndex = -1;
			name = "";
			localTransform = Matrix4x4::Identity();
			globalTransform = Matrix4x4::Identity();
		}

		Bone(const Matrix4x4& bindMatrix) : Bone()
		{
			globalTransform = Matrix4x4();
		}

		Bone& operator=(const Bone& rhs)
		{
			if (this != &rhs)
			{
				this->index = rhs.index;

				this->parentIndex = rhs.parentIndex;
				this->name = rhs.name;

				this->globalTransform = rhs.globalTransform;
				this->localTransform = rhs.localTransform;
			}
			return *this;
		}
	};
}
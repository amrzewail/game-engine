#pragma once

#include "Vector.h"
#include "Vector2.h"
#include "animation/VertexBone.hpp"

struct Vertex
{
public:
	Vector position;
	Vector normal;
	Vector2 texCoords;
	int boneIds[4];
	float boneWeights[4];

	Vertex()
	{
		position = Vector();
		normal = Vector();
		texCoords = Vector2();
	}

	Vertex(const Vertex& source)
	{
		position = Vector(source.position);
		normal = Vector(source.normal);
		texCoords = Vector2(source.texCoords);
	}

	Vertex& operator=(const Vertex& rhs)
	{
		if (this != &rhs)
		{
			position = Vector(rhs.position);
			normal = Vector(rhs.normal);
			texCoords = Vector2(rhs.texCoords);
		}
		return *this;
	}
};

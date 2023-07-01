#pragma once
#include "Vector.h"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"


class Transform
{
private:
	Matrix4x4* _localToWorldMatrix = 0;

public:
	Vector position;
	Quaternion rotation;
	Vector localScale;

	Transform()
	{
		position = Vector(0, 0, 0);
		rotation = Quaternion(1, 0, 0, 0);
		localScale = Vector(1, 1, 1);
	}

	~Transform()
	{
		delete _localToWorldMatrix;
	}


	Matrix4x4& LocalToWorldMatrix()
	{
		Vector pos = Vector(position.x, position.y, position.z * -1);
		_localToWorldMatrix = Matrix4x4::TRS(pos, rotation, localScale);
		return *_localToWorldMatrix;
	}

	Vector EulerAngles()
	{
		return *rotation.EulerAngles();
	}

	Vector Forward()
	{
		return *new Vector();
	}

};
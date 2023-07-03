#pragma once
#include "Vector.h"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"


class Transform
{
private:
	Matrix4x4* _localToWorldMatrix = 0;
	Matrix4x4* _positionMatrix = 0;
	Matrix4x4* _rotationMatrix = 0;
	Vector* _direction;

public:
	Vector* position;
	Quaternion* rotation;
	Vector* localScale;

	Transform()
	{
		position = new Vector(0, 0, 0);
		rotation = new Quaternion(1, 0, 0, 0);
		localScale = new Vector(1, 1, 1);
		_direction = new Vector();
	}

	~Transform()
	{
		delete position;
		delete rotation;
		delete localScale;
		delete _localToWorldMatrix;
		delete _positionMatrix;
		delete _rotationMatrix;
		delete _direction;
	}


	Matrix4x4& LocalToWorldMatrix()
	{
		Vector pos = Vector(position->x, position->y, position->z * -1);
		_localToWorldMatrix = Matrix4x4::TRS(pos, *rotation, *localScale);
		return *_localToWorldMatrix;
	}

	Matrix4x4& PositionMatrix()
	{
		Vector pos = Vector(position->x, position->y, position->z * -1);
		_positionMatrix = Matrix4x4::Translate(pos);
		return *_positionMatrix;
	}

	Vector EulerAngles()
	{
		return rotation->EulerAngles();
	}

	Vector& Forward()
	{
		Vector dir = (*rotation* Vector(0, 0, 1));
		return dir;
		//*_direction = (* rotation * Vector(0, 0, 1));
		//return *_direction;
	}

	Vector& Right()
	{
		Vector dir = (*rotation * Vector(1, 0, 0));
		return dir;
	}

	Vector& Up()
	{
		Vector dir = (*rotation * Vector(0, 1, 0));
		return dir;
	}

	Vector& Backward()
	{
		return -Forward();
	}

	Vector& Left()
	{
		return -Right();
	}

	Vector& Down()
	{
		return -Up();
	}

};
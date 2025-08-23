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
	Quaternion* _rotation;
	Vector* _eulerAngles;

public:
	Vector position;
	Vector localScale;

	Transform()
	{
		position = Vector(0, 0, 0);
		localScale = Vector(1, 1, 1);

		_rotation = new Quaternion(1, 0, 0, 0);
		_eulerAngles = new Vector(0, 0, 0);

		_localToWorldMatrix = new Matrix4x4();
		_positionMatrix = new Matrix4x4();
		_rotationMatrix = new Matrix4x4();

		_direction = new Vector();
	}

	~Transform()
	{
		delete _rotation;
		delete _eulerAngles;

		delete _localToWorldMatrix;
		delete _positionMatrix;
		delete _rotationMatrix;

		delete _direction;
	}


	Matrix4x4& LocalToWorldMatrix()
	{
		Vector pos = Vector(position.x, position.y, position.z);
		*_localToWorldMatrix = Matrix4x4::TRS(pos, *_rotation, localScale);
		return *_localToWorldMatrix;
	}

	Matrix4x4& PositionMatrix()
	{
		Vector pos = Vector(position.x, position.y, position.z);
		*_positionMatrix = Matrix4x4::Translate(pos);
		return *_positionMatrix;
	}

	void FromTRS(Matrix4x4& TRS)
	{

	}

	void set_Rotation(const Quaternion& quaternion)
	{
		*_rotation = quaternion.Normalized();
		*_eulerAngles = _rotation->EulerAngles();
	}

	void set_EulerAngles(const Vector& eulerAngles)
	{
		*_rotation = Quaternion::Euler(eulerAngles);
		*_eulerAngles = eulerAngles;
	}

	void set_EulerAngles(float x, float y, float z)
	{
		set_EulerAngles(Vector(x, y, z));
	}

	Quaternion get_Rotation()
	{
		return *_rotation;
	}

	Vector get_EulerAngles()
	{
		return *_eulerAngles;
	}

	Vector Forward() const
	{
		Vector dir = (*_rotation * Vector(0, 0, 1));
		return dir;
	}

	Vector Right() const
	{
		Vector dir = (*_rotation * Vector(1, 0, 0));
		return dir;
	}

	Vector Up() const
	{
		Vector dir = (*_rotation * Vector(0, 1, 0));
		return dir;
	}

	Vector Back() const
	{
		return -Forward();
	}

	Vector Left() const
	{
		return -Right();
	}

	Vector Down() const
	{
		return -Up();
	}

};
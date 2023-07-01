#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Quaternion.hpp"

using namespace glm;

struct Matrix4x4
{

private:

	mat4* _matrix;

	mat4 Value()
	{
		return *_matrix;
	}

public:

	static Matrix4x4* Identity()
	{
		return new Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}

	static Matrix4x4* Zero()
	{
		return new Matrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	static Matrix4x4* Rotate(Quaternion& quaternion)
	{
		Quaternion q = *quaternion.Normalized();

		float q0 = q.W();
		float q1 = q.X();
		float q2 = q.Y();
		float q3 = q.Z();

		float q0q0 = q0 * q0;
		float q0q1 = q0 * q1;
		float q0q2 = q0 * q2;
		float q0q3 = q0 * q3;

		float q1q0 = q1 * q0;
		float q1q1 = q1 * q1;
		float q1q2 = q1 * q2;
		float q1q3 = q1 * q3;

		float q2q0 = q2 * q0;
		float q2q1 = q2 * q1;
		float q2q2 = q2 * q2;
		float q2q3 = q2 * q3;

		float q3q0 = q3 * q0;
		float q3q1 = q3 * q1;
		float q3q2 = q3 * q2;
		float q3q3 = q3 * q3;

		float m11 = 2 * (q0q0 + q1q1) - 1;
		float m12 = 2 * (q1q2 - q0q3);
		float m13 = 2 * (q1q3 + q0q2);

		float m21 = 2 * (q1q2 + q0q3);
		float m22 = 2 * (q0q0 + q2q2) - 1;
		float m23 = 2 * (q2q3 - q0q1);

		float m31 = 2 * (q1q3 - q0q2);
		float m32 = 2 * (q2q3 + q0q1);
		float m33 = 2 * (q0q0 + q3q3) - 1;

		Matrix4x4* m = new Matrix4x4(
			m11, m12, m13, 0,
			m21, m22, m23, 0,
			m31, m32, m33, 0,
			0, 0, 0, 1
		);

		return m;
	}

	static Matrix4x4* Translate(Vector& vector)
	{
		return new Matrix4x4(
			1, 0, 0, vector.x,
			0, 1, 0, vector.y,
			0, 0, 1, vector.z,
			0, 0, 0, 1
		);
	}

	static Matrix4x4* Scale(Vector& vector)
	{
		return new Matrix4x4(
			vector.x, 0, 0, 0,
			0, vector.y, 0, 0,
			0, 0, vector.z, 0,
			0, 0, 0, 1
		);
	}

	static Matrix4x4* TRS(Vector& translation, Quaternion& rotation, Vector& scale)
	{
		Matrix4x4 t = *Translate(translation);
		Matrix4x4 r = *Rotate(rotation);
		Matrix4x4 s = *Scale(scale);

		Matrix4x4* trs = s * *(r * t);

		return trs;
	}

	Matrix4x4(	
		float x0, float y0, float z0, float w0,
		float x1, float y1, float z1, float w1,
		float x2, float y2, float z2, float w2,
		float x3, float y3, float z3, float w3)
	{
		_matrix = new mat4(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3);
	}

	~Matrix4x4()
	{
		delete _matrix;
	}

	float At(int row, int column)
	{
		return (*_matrix)[row][column];
	}

	std::string ToString()
	{
		std::string s;
		for (int row = 0; row < 4; ++row)
		{
			for (int column = 0; column < 4; ++column)
			{
				s += std::to_string(At(row, column));
				if (column < 3) s += ", ";
			}
			if (row < 3) s += '\n';
		}
		return s;
	}

	Matrix4x4* operator *(Matrix4x4& rhs) const
	{
		mat4 rhsM = rhs.Value();
		rhsM = (*_matrix) * rhsM;

		return new Matrix4x4(
			rhsM[0][0], rhsM[0][1], rhsM[0][2], rhsM[0][3],
			rhsM[1][0], rhsM[1][1], rhsM[1][2], rhsM[1][3],
			rhsM[2][0], rhsM[2][1], rhsM[2][2], rhsM[2][3],
			rhsM[3][0], rhsM[3][1], rhsM[3][2], rhsM[3][3]);

	}

};
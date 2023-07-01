#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Quaternion.hpp"

using namespace glm;

struct Matrix4x4
{

private:

	mat4* _matrix;
	float* _data;

	mat4 Value()
	{
		return *_matrix;
	}

	static Matrix4x4* FromMat4(glm::mat4 mat)
	{
		return new Matrix4x4(
			mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
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

		return FromMat4(glm::mat4_cast(glm::fquat(quaternion.W(), quaternion.X(), quaternion.Y(), quaternion.Z())));
	}

	static Matrix4x4* Translate(Vector& vector)
	{
		return FromMat4(glm::translate(mat4(1.0), vec3(vector.x, vector.y, vector.z)));
	}

	static Matrix4x4* Scale(Vector& vector)
	{
		return FromMat4(glm::scale(mat4(1.0), vec3(vector.x, vector.y, vector.z)));
	}

	static Matrix4x4* TRS(Vector& translation, Quaternion& rotation, Vector& scale)
	{
		mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));
		mat4 rotate = glm::mat4_cast(glm::fquat(rotation.W(), rotation.X(), rotation.Y(), rotation.Z()));
		mat4 scaleMat = glm::scale(glm::mat4(1.0), glm::vec3(scale.x, scale.y, scale.z));

		return FromMat4(trans * rotate * scaleMat);

	}

	static Matrix4x4* Perspective(float fov, float aspect, float zNear, float zFar)
	{
		fov = glm::radians(fov);
		return FromMat4(glm::perspective(fov, aspect, zNear, zFar));
	}

	static Matrix4x4* Inverse(Matrix4x4& matrix)
	{
		return FromMat4(glm::inverse(matrix.Value()));
	}

	static Matrix4x4* LookAt(Vector& from, Vector& to, Vector& up)
	{
		return FromMat4(glm::lookAt(glm::vec3(from.x, from.y, from.z), glm::vec3(to.x, to.y, to.z), glm::vec3(up.x, up.y, up.z)));
	}

	Matrix4x4(	
		float x0, float y0, float z0, float w0,
		float x1, float y1, float z1, float w1,
		float x2, float y2, float z2, float w2,
		float x3, float y3, float z3, float w3)
	{
		_matrix = new mat4(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3);

		_data = new float[4 * 4];
		int index = 0;
		for (int x = 0; x < 4; ++x)
		{
			for (int y = 0; y < 4; ++y)
			{
				_data[index++] = (*_matrix)[x][y];
			}
		}
	}

	~Matrix4x4()
	{
		delete _matrix;
		delete[] _data;
	}

	float At(int row, int column) const
	{
		return (*_matrix)[column][row];
	}

	float* Data() const
	{
		return _data;
	}

	std::string ToString() const
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

		return FromMat4(rhsM);

	}

};
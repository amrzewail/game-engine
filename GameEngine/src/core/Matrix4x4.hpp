#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Quaternion.hpp"
#include "Vector4.h"

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

	static Matrix4x4 FromMat4(glm::mat4 mat)
	{
		return Matrix4x4(
			mat[0][0], mat[1][0], mat[2][0], mat[3][0],
			mat[0][1], mat[1][1], mat[2][1], mat[3][1],
			mat[0][2], mat[1][2], mat[2][2], mat[3][2],
			mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
	}

public:

	static Matrix4x4 Identity()
	{
		Matrix4x4 mat;
		mat.SetIdentity();
		return mat;
	}

	static Matrix4x4 Zero()
	{
		return Matrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	static Matrix4x4 Rotate(const Quaternion quaternion)
	{
		return FromMat4(glm::mat4_cast(glm::fquat(quaternion.w, quaternion.x, quaternion.y, quaternion.z)));
	}

	static Matrix4x4 Translate(Vector vector)
	{
		Matrix4x4 m(1, 0, 0, vector.x,
			0, 1, 0, vector.y,
			0, 0, 1, vector.z,
			0, 0, 0, 1);
		return m;
	}

	static Matrix4x4 Scale(Vector vector)
	{
		return FromMat4(glm::scale(mat4(1.0), vec3(vector.x, vector.y, vector.z)));
	}

	static const Matrix4x4 TRS(Vector t, Quaternion r, Vector s)
	{
		Vector position = Vector(t.x, t.y, t.z);
		Quaternion rotation = r;
		Vector scale = s;

		Vector4 c0(rotation * Vector(scale.x, 0, 0), 0);
		Vector4 c1(rotation * Vector(0, scale.y, 0), 0);
		Vector4 c2(rotation * Vector(0, 0, scale.z), 0);
		Vector4 c3(position, 1);

		Matrix4x4 m(c0, c1, c2, c3);

		return m;
	}

	static Matrix4x4 Perspective(float fov, float aspect, float zNear, float zFar)
	{
		fov = glm::radians(fov);
		Matrix4x4 mat = FromMat4(glm::perspectiveLH(fov, aspect, zNear, zFar));

		//mat.Set(0, 2, -mat.At(0, 2));
		//mat.Set(1, 2, -mat.At(1, 2));
		//mat.Set(2, 2, -mat.At(2, 2));
		//mat.Set(3, 2, -mat.At(3, 2));

		return mat;
	}

	static Matrix4x4 Inverse(Matrix4x4& matrix)
	{
		return FromMat4(glm::inverse(matrix.Value()));
	}

	static Matrix4x4 LookAt(Vector& from, Vector& to, Vector& up)
	{
		return FromMat4(glm::lookAt(glm::vec3(from.x, from.y, from.z), glm::vec3(to.x, to.y, to.z), glm::vec3(up.x, up.y, up.z)));
	}

	//row,column
	Matrix4x4(
		float c00, float c01, float c02, float c03,
		float c10, float c11, float c12, float c13,
		float c20, float c21, float c22, float c23,
		float c30, float c31, float c32, float c33)
	{
		_matrix = new mat4();
		_data = new float[4 * 4];
		Set(c00, c01, c02, c03, c10, c11, c12, c13, c20, c21, c22, c23, c30, c31, c32, c33);
	}


	Matrix4x4() : Matrix4x4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
	{
	}

	Matrix4x4(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3) : Matrix4x4(	c0.x, c1.x, c2.x, c3.x,
																										c0.y, c1.y, c2.y, c3.y,
																										c0.z, c1.z, c2.z, c3.z, 
																										c0.w, c1.w, c2.w, c3.w) {}


	Matrix4x4(const Matrix4x4& source) : Matrix4x4()
	{
		*_matrix = mat4(*source._matrix);
		for (int i = 0; i < 16; i++) _data[i] = source._data[i];
	}

	Matrix4x4& operator=(const Matrix4x4& rhs)
	{
		if (this == &rhs) return *this;
		*_matrix = mat4(*rhs._matrix);
		for (int i = 0; i < 16; i++) _data[i] = rhs._data[i];
		return *this;
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

	void Set(const int row, const int column, const float value)
	{
		_matrix->operator[](column)[row] = value;
		_data[column * 4 + row] = value;
	}

	void Set(float c00, float c01, float c02, float c03,
		float c10, float c11, float c12, float c13,
		float c20, float c21, float c22, float c23,
		float c30, float c31, float c32, float c33)
	{
		Set(0, 0, c00);
		Set(0, 1, c01);
		Set(0, 2, c02);
		Set(0, 3, c03);

		Set(1, 0, c10);
		Set(1, 1, c11);
		Set(1, 2, c12);
		Set(1, 3, c13);

		Set(2, 0, c20);
		Set(2, 1, c21);
		Set(2, 2, c22);
		Set(2, 3, c23);

		Set(3, 0, c30);
		Set(3, 1, c31);
		Set(3, 2, c32);
		Set(3, 3, c33);
	}

	Vector GetColumn(int columnIndex)
	{
		return Vector(At(0, columnIndex), At(1, columnIndex), At(2, columnIndex));
	}

	Vector GetTranslation()
	{
		return GetColumn(3);
	}

	//Vector GetScale()
	//{
	//	float x = glm::sqrt(At(0, 0) * At(0, 0) + At(1, 0) * At(1, 0) + At(2, 0) * At(2, 0));
	//	float y = glm::sqrt(At(0, 1) * At(0, 1) + At(1, 1) * At(1, 1) + At(2, 1) * At(2, 1));
	//	float z = glm::sqrt(At(0, 2) * At(0, 2) + At(1, 2) * At(1, 2) + At(2, 2) * At(2, 2));
	//	return Vector(x, y, z);
	//}

	Quaternion GetRotation()
	{
		fquat quat = glm::quat_cast(*_matrix);
		return Quaternion(quat.w, quat.x, quat.y, quat.z);
	}

	//void ExtractTRS(Vector& position, Quaternion& rotation, Vector& scale)
	//{

	//}

	void SetIdentity()
	{
		Set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	}

	void SetInverse()
	{
		*_matrix = glm::inverse(Value());
		int index = 0;
		for (int x = 0; x < 4; ++x)
		{
			for (int y = 0; y < 4; ++y)
			{
				_data[index++] = (*_matrix)[x][y];
			}
		}
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

	void MultiplyVector(Vector* rhs)
	{
		Vector result;
		result.x = At(0, 0) * rhs->x + At(0, 1) * rhs->y + At(0, 2) * rhs->z;
		result.y = At(1, 0) * rhs->x + At(1, 1) * rhs->y + At(1, 2) * rhs->z;
		result.z = At(2, 0) * rhs->x + At(2, 1) * rhs->y + At(2, 2) * rhs->z;

		rhs->x = result.x;
		rhs->y = result.y;
		rhs->z = result.z;
	}

	Matrix4x4 operator *(Matrix4x4& rhs) const
	{
		mat4 rhsM = rhs.Value();
		rhsM = (*_matrix) * rhsM;

		return FromMat4(rhsM);

	}

	Matrix4x4 operator *(float rhs) const
	{
		*_matrix = *_matrix * rhs;
		return FromMat4(*_matrix);
	}

	Matrix4x4 operator +=(Matrix4x4 rhs) const
	{
		*_matrix += rhs.Value();
		return FromMat4(*_matrix);
	}

	Vector operator* (const Vector& rhs)
	{
		Vector result;
		result.x = At(0, 0) * rhs.x + At(0, 1) * rhs.y + At(0, 2) * rhs.z;
		result.y = At(1, 0) * rhs.x + At(1, 1) * rhs.y + At(1, 2) * rhs.z;
		result.z = At(2, 0) * rhs.x + At(2, 1) * rhs.y + At(2, 2) * rhs.z;
		return result;
	}

};
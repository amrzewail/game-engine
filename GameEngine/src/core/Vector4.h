#pragma once

#include <string>

struct Vector4
{
private:

public:
	float x;
	float y;
	float z;
	float w;



	Vector4()
	{
		x = y = z = w = 0;
	}

	Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4(const Vector& v, const float w) : Vector4(v.x, v.y, v.z, w){}

	Vector4(const Vector4& source)
	{
		this->x = source.x;
		this->y = source.y;
		this->z = source.z;
		this->w = source.w;
	}


	static Vector4 Zero()
	{
		return Vector4(0, 0, 0, 0);
	}

	static Vector4 One()
	{
		return Vector4(1, 1, 1, 1);
	}

	static float Dot(const Vector4& v1, const Vector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	//static Vector4 Cross(const Vector4& v1, const Vector4& v2)
	//{
	//	Vector4 v;
	//	v.x = v1.y * v2.z - v1.z * v2.y;
	//	v.y = v1.z * v2.x - v1.x * v2.z;
	//	v.z = v1.x * v2.y - v1.y * v2.x;
	//	return v;
	//}

	static float Angle(const Vector4& v1, const Vector4& v2)
	{
		float ab = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
		return acosf(ab / (v1.Magnitude() * v2.Magnitude()));
	}

	float Magnitude() const
	{
		return sqrtf(MagnitudeSquared());
	}

	float MagnitudeSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}


	std::string ToString()
	{
		return '(' + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ')';
	}


	Vector4& operator =(const Vector4& rhs)
	{
		if (this != &rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			this->w = rhs.w;
		}
		return *this;
	}


	Vector4& operator+= (const Vector4& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	}


	Vector4& operator-= (const Vector4& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;
		return *this;
	}

	friend Vector4& operator -(Vector4& value)
	{
		value.x *= -1;
		value.y *= -1;
		value.z *= -1;
		value.w *= -1;
		return value;
	}


	friend Vector4 operator +(Vector4 lhs, Vector4& rhs)
	{
		lhs += rhs;
		return lhs;
	}


	Vector4& operator* (const float& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		this->w *= rhs;
		return *this;
	}
};

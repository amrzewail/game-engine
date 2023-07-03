#pragma once

#include <string>

struct Vector
{
private:

public:
	float x;
	float y;
	float z;

	Vector()
	{
		x = y = z = 0;
	}

	Vector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector& operator =(const Vector& rhs)
	{
		if (this != &rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
		}
		return *this;
	}

	Vector(const Vector& source)
	{
		this->x = source.x;
		this->y = source.y;
		this->z = source.z;
	}


	std::string ToString()
	{
		return '(' + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ')';
	}


	Vector& operator+= (const Vector& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}


	Vector& operator-= (const Vector& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	friend Vector& operator -(Vector& value)
	{
		value.x *= -1;
		value.y *= -1;
		value.z *= -1;
		return value;
	}


	friend Vector operator +(Vector lhs, Vector& rhs)
	{
		lhs += rhs;
		return lhs;
	}


	Vector& operator* (const float& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		return *this;
	}
};

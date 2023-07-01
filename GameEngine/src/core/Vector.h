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

	std::string ToString()
	{
		return '(' + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ')';
	}


	Vector* operator +(Vector& rhs) const
	{
		return new Vector(x + rhs.x, y + rhs.y, z + rhs.z);
	}
};

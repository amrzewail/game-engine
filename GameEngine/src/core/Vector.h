#pragma once

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
};

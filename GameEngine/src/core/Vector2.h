#pragma once

struct Vector2
{
private:

public:
	float x;
	float y;

	Vector2()
	{
		x = y = 0;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

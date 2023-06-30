#pragma once

struct Color
{
private:

public:
	float r;
	float g;
	float b;
	float a;

	Color()
	{
		r = g = b = a = 0;
	}

	Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

};

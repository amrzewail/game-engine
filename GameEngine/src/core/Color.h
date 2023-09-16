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

	Color(const Color& rhs)
	{
		this->r = rhs.r;
		this->g = rhs.g;
		this->b = rhs.b;
		this->a = rhs.a;
	}

	Color& operator =(const Color& rhs)
	{
		if (this != &rhs)
		{
			this->r = rhs.r;
			this->g = rhs.g;
			this->b = rhs.b;
			this->a = rhs.a;
		}
		return *this;
	}

	Color operator *(float value)
	{
		return Color(r * value, g * value, b * value, a * value);
	}

};

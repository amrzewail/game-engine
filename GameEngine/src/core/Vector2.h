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

	Vector2& operator =(const Vector2& rhs)
	{
		if (this != &rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
		}
		return *this;
	}

	Vector2(const Vector& source)
	{
		this->x = source.x;
		this->y = source.y;
	}

	std::string ToString()
	{
		return '(' + std::to_string(x) + ", " + std::to_string(y) + ')';
	}


	Vector2* operator +(Vector2& rhs) const
	{
		return new Vector2(x + rhs.x, y + rhs.y);
	}
};

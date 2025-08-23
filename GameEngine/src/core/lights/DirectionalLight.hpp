#pragma once

#include "Light.hpp"
#include "../Vector.h"

class DirectionalLight : public Light
{
public:

	Vector direction;

	DirectionalLight() : Light()
	{
		type = Directional;
		direction = Vector(0, 1, 0);
	}

	~DirectionalLight()
	{

	}
};
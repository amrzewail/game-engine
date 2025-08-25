#pragma once

#include <Moths/API.h>
#include <Moths/Core/Vector.h>
#include <Moths/Lighting/Light.h>

namespace MothsEngine
{
	class MOTHS_API DirectionalLight : public Light
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
}
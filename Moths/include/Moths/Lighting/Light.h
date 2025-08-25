#pragma once

#include <Moths/API.h>
#include <Moths/Core/Color.h>
#include <Moths/Lighting/LightType.h>

namespace MothsEngine
{
	class MOTHS_API Light
	{
	public:
		Color color;
		float intensity;
		LightType type;
		bool castShadows;


		Light();


		virtual ~Light();
	};
}
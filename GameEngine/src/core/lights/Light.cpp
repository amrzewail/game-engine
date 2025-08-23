#include "Light.hpp"
#include "Lights.h"


Light::Light()
{
	color = Color(1, 1, 1, 1);
	intensity = 0;

	Lights::AddLight(this);
}

Light::~Light()
{
	Lights::RemoveLight(this);
}


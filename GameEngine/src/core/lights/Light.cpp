#include "Light.hpp"
#include "Lights.h"


Light::Light()
{
	color = new Color(1, 1, 1, 1);
	intensity = 0;

	Lights::AddLight(this);
}

Light::~Light()
{
	delete color;

	Lights::RemoveLight(this);
}


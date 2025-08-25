#include <Moths/Lighting/Light.h>
#include <Moths/Lighting/Lights.h>

using namespace MothsEngine;
using namespace MothsEngine::Internal;

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


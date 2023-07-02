#pragma once
#include "../Color.h"
#include "LightType.h"

class Lights;

class Light
{
public:
	Color* color;
	float intensity;
	LightType type;
	bool castShadows;


	Light();


	virtual ~Light();
};
#pragma once

#include <vector>
#include "Light.hpp"

class Lights
{
private:
	static std::vector<Light*>* _lights;

public:
	static void AddLight(Light* light)
	{
		if (!_lights)
		{
			_lights = new std::vector<Light*>();
		}
		_lights->push_back(light);
	}

	static void RemoveLight(Light* light)
	{
		if (!_lights)
		{
			return;
		}

		for (int i = 0; i < _lights->size(); i++)
		{
			if ((* _lights)[i] == light)
			{
				_lights->erase(_lights->begin() + i);
				break;
			}
		}
	}

	static std::vector<Light*>& GetLights()
	{
		if (!_lights)
		{
			_lights = new std::vector<Light*>();
		}
		return *_lights;
	}
};
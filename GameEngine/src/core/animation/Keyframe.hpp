#pragma once
#include "KeyframeProperty.hpp"

struct Keyframe
{

private:
	std::unordered_map<std::string, KeyframeProperty*> _properties;
	int _tick;


public:

	Keyframe(int tick)
	{
		_tick = tick;
	}

	~Keyframe()
	{
		for (auto p : _properties)
		{
			delete p.second;
		}
		_properties.clear();
	}

	Keyframe(const Keyframe& source)
	{
		if (this == &source) return;

		_tick = source._tick;
		for (auto p : _properties) delete p.second;
		_properties.clear();
		for (auto p : source._properties)
		{
			_properties[p.first] = new KeyframeProperty(*p.second);
		}
	}

	Keyframe& operator=(const Keyframe& rhs)
	{
		if (this == &rhs) return *this;
		_tick = rhs._tick;
		for (auto p : _properties) delete p.second;
		_properties.clear();
		for (auto p : rhs._properties)
		{
			_properties[p.first] = new KeyframeProperty(*p.second);
		}
		return *this;
	}

	const int Tick()
	{
		return _tick;
	}

	void AddProperty(std::string name, KeyframeProperty p)
	{
		_properties[name] = new KeyframeProperty(p);
	}

	KeyframeProperty GetProperty(std::string name)
	{
		if (_properties.find(name) != _properties.end())
		{
			return *_properties[name];
		}
		return KeyframeProperty();
	}

	const std::unordered_map<std::string, KeyframeProperty*> GetProperties()
	{
		return _properties;
	}
};
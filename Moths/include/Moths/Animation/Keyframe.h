#pragma once

#include <unordered_map>

#include <Moths/API.h>
#include <Moths/Animation/KeyframeProperty.h>

namespace MothsEngine
{
	struct MOTHS_API Keyframe
	{

	private:
		std::unordered_map<std::string, KeyframeProperty*> _properties;
		int _tick;


	public:

		Keyframe(int tick)
		{
			_properties = std::unordered_map<std::string, KeyframeProperty*>();
			_properties.clear();
			_tick = tick;
		}

		~Keyframe()
		{
			for (auto p : _properties) delete p.second;
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

		inline Keyframe& operator=(const Keyframe& rhs)
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

		inline const int Tick()
		{
			return _tick;
		}

		inline void AddProperty(std::string name, KeyframeProperty p)
		{
			_properties[name] = new KeyframeProperty(p);
		}

		inline KeyframeProperty GetProperty(std::string name)
		{
			if (_properties.find(name) != _properties.end())
			{
				return *_properties[name];
			}
			return KeyframeProperty();
		}

		inline const std::unordered_map<std::string, KeyframeProperty*> GetProperties()
		{
			return _properties;
		}
	};
}
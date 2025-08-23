#pragma once
#include <vector>
#include "Keyframe.hpp"


class Animation
{

private:
	std::string _name;
	float _duration;
	int _ticksPerSecond;
	std::vector<Keyframe*> _keyframes;

public:
	bool loopTime;

	~Animation()
	{
		for (int i = 0; i < _keyframes.size(); i++)
		{
			delete _keyframes[i];
		}
		_keyframes.clear();
	}

	void SetName(std::string name)
	{
		_name = name;
	}

	void SetDuration(float duration)
	{
		_duration = duration;
	}

	void SetTicksPerSecond(int ticksPerSecond)
	{
		_ticksPerSecond = ticksPerSecond;
	}

	Keyframe* AddKeyframe(Keyframe keyframe)
	{
		auto key = new Keyframe(keyframe);
		_keyframes.push_back(key);
		return key;
	}

	Keyframe* GetKeyframe(int tick)
	{
		for (int i = 0; i < _keyframes.size(); i++)
		{
			if (_keyframes[i]->Tick() == tick) return _keyframes[i];
		}
		return AddKeyframe(Keyframe(tick));
	}
};
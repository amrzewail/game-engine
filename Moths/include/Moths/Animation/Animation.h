#pragma once

#include <vector>
#include <string>
#include <Moths/API.h>
#include <Moths/Animation/Keyframe.h>

namespace MothsEngine
{
	class MOTHS_API Animation
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
				if (!_keyframes[i]) continue;
				delete _keyframes[i];
			}
			_keyframes.clear();
		}

		inline void SetName(std::string name)
		{
			_name = name;
		}

		inline void SetDuration(float duration)
		{
			_duration = duration;
		}

		inline void SetTicksPerSecond(int ticksPerSecond)
		{
			_ticksPerSecond = ticksPerSecond;
		}

		inline Keyframe* AddKeyframe(Keyframe keyframe)
		{
			auto key = new Keyframe(keyframe);
			_keyframes.push_back(key);
			return key;
		}

		inline Keyframe* GetKeyframe(int tick)
		{
			for (int i = 0; i < _keyframes.size(); i++)
			{
				if (_keyframes[i]->Tick() == tick) return _keyframes[i];
			}
			return AddKeyframe(Keyframe(tick));
		}
	};
}
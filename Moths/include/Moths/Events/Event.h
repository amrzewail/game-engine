#pragma once

#include <vector>

#include <Moths/API.h>

namespace MothsEngine
{
	template <typename T>
	class MOTHS_API EventBase
	{
	public:
		void AddListener(T listener);
		void RemoveListener(T listener);
		void RemoveAllListeners();
		void operator +=(T listener);
		void operator -=(T listener);
	protected:
		std::vector<T> _listeners;
	};

	template <typename T0, typename T1, typename T2>
	class Event3 : public EventBase<void(*)(T0, T1, T2)>
	{
	public:
		void Invoke(T0 p0, T1 p1, T2 p2);
		void operator=(void(*listener)(T0, T1, T2));
	};

	template <typename T0, typename T1>
	class Event2 : public EventBase<void(*)(T0, T1)>
	{
	public:
		void Invoke(T0 p0, T1 p1);
		void operator=(void(*listener)(T0, T1));
	};

	template <typename T0>
	class Event1 : public EventBase<void(*)(T0)>
	{
	public:
		void Invoke(T0 p0);
		void operator=(void(*listener)(T0));
	};

	class Event : public EventBase<void(*)()>
	{
	public:
		void Invoke();
		void operator=(void(*listener)());

	};
}

#include "Event.cpp"

#include "Event.h"

template <typename T>
void EventBase<T>::AddListener(T listener)
{
	_listeners.push_back(listener);
}

template <typename T>
void EventBase<T>::RemoveListener(T listener)
{
	for (int i = 0; i < _listeners.size(); ++i)
	{
		if (_listeners[i] == listener)
		{
			_listeners.erase(_listeners.begin() + i);
			break;
		}
	}
}

template<typename T>
void EventBase<T>::RemoveAllListeners()
{
	_listeners.clear();
}

template<typename T>
void EventBase<T>::operator+=(T listener)
{
	AddListener(listener);
}
template<typename T>
void EventBase<T>::operator-=(T listener)
{
	RemoveListener(listener);
}


void Event::Invoke()
{
	for (auto listener : _listeners) listener();
}

void Event::operator=(void(*listener)())
{
	this->RemoveAllListeners();
	this->AddListener(listener);
}

template <typename T0>
void Event1<T0>::Invoke(T0 p0)
{
	for (auto listener : this->_listeners) listener(p0);
}

template<typename T0>
void Event1<T0>::operator=(void(*listener)(T0))
{
	this->RemoveAllListeners();
	this->AddListener(listener);
}

template <typename T0, typename T1>
void Event2<T0, T1>::Invoke(T0 p0, T1 p1)
{
	for (auto listener : this->_listeners) listener(p0, p1);
}

template<typename T0, typename T1>
void Event2<T0, T1>::operator=(void(*listener)(T0, T1))
{
	this->RemoveAllListeners();
	this->AddListener(listener);
}

template <typename T0, typename T1, typename T2>
void Event3<T0, T1, T2>::Invoke(T0 p0, T1 p1, T2 p2)
{
	for (auto listener : this->_listeners) listener(p0, p1, p2);
}

template<typename T0, typename T1, typename T2>
void Event3<T0, T1, T2>::operator=(void(*listener)(T0, T1, T2))
{
	this->RemoveAllListeners();
	this->AddListener(listener);
}

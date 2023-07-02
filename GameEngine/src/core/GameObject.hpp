#pragma once

#include <vector>
#include <unordered_map>

#include "Camera.hpp"
#include "Transform.hpp"
#include "Component.hpp"

class GameObject
{
private:
	std::unordered_map<std::string, Component*>* _components = new std::unordered_map<std::string, Component*>();

public:
	Transform* transform;

	GameObject();

	~GameObject();



	void AddComponent(Component* component);


	template <typename T>
	T& AddComponent()
	{
		T* component = new T();
		Component* c = (Component*)component;

		AddComponent(c);
		return *component;

	}

	template <typename T>
	void RemoveComponent()
	{

	}

};
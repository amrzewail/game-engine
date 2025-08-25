#pragma once

#include <vector>
#include <unordered_map>

#include <Moths/API.h>

#include <Moths/Core/Transform.h>
#include <Moths/Component/Component.h>
#include <Moths/Entity/Camera.h>

namespace MothsEngine
{

	class MOTHS_API GameObject
	{
	private:
		std::unordered_map<std::string, std::vector<Component*>> _components = std::unordered_map<std::string, std::vector<Component*>>();

	public:
		Transform* transform;

		GameObject();

		~GameObject();

		template <typename T>
		T& AddComponent()
		{
			T* component = new T();
			Component* c = (Component*)component;

			std::string identifier = typeid(T).name();
			if (_components.find(identifier) == _components.end()) _components[identifier] = std::vector<Component*>();
			_components[identifier].push_back(component);

			component->gameObject = this;
			component->Awake();

			return *component;
		}

		template <typename T>
		void RemoveComponent(int index = 0)
		{
			std::string identifier = typeid(T).name();
			if (_components.find(identifier) == _components.end()) return nullptr;
			auto list = _components[identifier];
			if (list.size() <= index) return nullptr;
			list.erase(list.begin() + index);
		}

		template <typename T>
		T* GetComponent(int index = 0)
		{
			std::string identifier = typeid(T).name();
			if (_components.find(identifier) == _components.end()) return nullptr;
			auto list = _components[identifier];
			if (list.size() <= index) return nullptr;
			return dynamic_cast<T*>(list[index]);
		}

		template <typename T>
		std::vector<T*> GetComponents()
		{
			std::string identifier = typeid(T).name();
			if (_components.find(identifier) == _components.end()) std::vector<T*>();

			std::vector<T*> list;
			for (auto c : _components[identifier]) list.push_back(dynamic_cast<T*>(c));

			return list;
		}
	};
}
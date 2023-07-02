#include "GameObject.hpp"

GameObject::GameObject()
{
	_components = new std::unordered_map<std::string, Component*>();
	transform = new Transform();
}


GameObject::~GameObject()
{
	for (auto& pair : *_components)
	{
		pair.second->OnDestroy();
		delete pair.second;
	}

	delete _components;
	delete transform;
}

void GameObject::AddComponent(Component* component)
{
	(* _components)[component->runtimeGuid->ToString()] = component;
	component->gameObject = this;
	component->Awake();
}

void GameObject::Render(Camera& camera) const
{
	for (auto& pair : *_components)
	{
		pair.second->Render(camera);
	}
}

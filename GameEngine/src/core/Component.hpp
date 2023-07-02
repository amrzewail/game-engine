#pragma once

#include "Camera.hpp"
#include "utils/Guid.hpp"

class GameObject;

class Component
{

public:
	const Guid* runtimeGuid;
	GameObject* gameObject;


	Component();

	virtual ~Component();

	virtual void Awake();

	virtual void Start();

	virtual void Update(float deltaTime);


	virtual void Render(Camera& targetCamera) const;

	virtual void OnDestroy();
};
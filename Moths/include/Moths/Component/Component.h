#pragma once

#include <Moths/API.h>
#include <Moths/Core/Guid.h>
#include <Moths/Entity/Camera.h>

namespace MothsEngine
{
	class GameObject;

	class MOTHS_API Component
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
}
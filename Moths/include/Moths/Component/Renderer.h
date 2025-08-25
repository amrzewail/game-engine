#pragma once

#include <iostream>

#include <Moths/API.h>

#include <Moths/Entity/Camera.h>
#include <Moths/Component/Component.h>
#include <Moths/Lighting/Lights.h>
#include <Moths/Core/Shader.h>
#include <Moths/Lighting/DirectionalLight.h>
#include <Moths/Core/Screen.h>
#include <Moths/Entity/GameObject.h>
#include <Moths/Core/Material.h>

namespace MothsEngine
{
	class MOTHS_API Renderer : public Component
	{
	public:

		std::vector<Material*> materials;
		int* priority = new int(0);

		virtual ~Renderer() = 0;

		Renderer();


		virtual void Render(Camera& camera);

		virtual void PreDraw(Camera& camera, Material* material, int submeshIndex) = 0;

		virtual void Draw(Camera& camera, int index) = 0;

		virtual void SetMaterial(Material* material);
	};
}
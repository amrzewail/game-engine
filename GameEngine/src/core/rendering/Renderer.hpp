#pragma once

#include <iostream>

#include "../Camera.hpp"
#include "../Component.hpp"
#include "../lights/Lights.h"
#include "../shaders/Shader.h"
#include "../lights/DirectionalLight.hpp"
#include "../Screen.h"
#include "../GameObject.hpp"
#include "../Material.hpp"


class Renderer : public Component
{
public:

	std::vector<Material*> materials;
	int* priority = new int(0);

	virtual ~Renderer() = 0;

	Renderer();


	virtual void Render(Camera& camera);

	virtual void PreDraw(Camera& camera, Material* material, int submeshIndex) = 0;

	virtual void Draw(Camera& camera, int index) = 0;
};
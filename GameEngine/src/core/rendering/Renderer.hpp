#pragma once
#include "../Camera.hpp"

class Renderer
{
public:

	int* priority = new int(0);

	virtual ~Renderer() = 0;

	Renderer();

	virtual void Render(Camera& camera) const = 0;
};
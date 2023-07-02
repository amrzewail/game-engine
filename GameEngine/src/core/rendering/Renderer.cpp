#include "Renderer.hpp"
#include "Renderers.hpp"
#include <iostream>


Renderer::~Renderer()
{
	Renderers::RemoveRenderer(this);

	delete priority;
}

Renderer::Renderer()
{
	*priority = 0;
	Renderers::AddRenderer(this);
}



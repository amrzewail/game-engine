#pragma once

#include <Moths/API.h>
#include <Moths/Entity/Camera.h>

namespace MothsEngine
{
	class MOTHS_API IRenderPass
	{

	public:

		unsigned int activeTextureIndex;
		unsigned int frameBuffer;
		unsigned int textureBuffer;


		virtual ~IRenderPass() {}

		virtual void Initialize(int index) = 0;

		virtual void Render(const IRenderPass& lastPass, Camera& camera) = 0;

	};
}
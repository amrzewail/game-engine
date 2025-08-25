#pragma once

#include <glad/glad.h>
#include <cstddef>

#include <Moths/API.h>

#include <Moths/Core/Shader.h>
#include <Moths/Core/Quad.h>
#include <Moths/Core/Screen.h>

#include <Moths/Rendering/IRenderPass.h>

#include <Moths/Entity/Camera.h>

#include <Moths/AssetManagement/ShaderAsset.h>

namespace MothsEngine
{
	class MOTHS_API FXAARenderPass : public IRenderPass
	{
	private:
		Shader* _fxaaShader;

	public:

		FXAARenderPass()
		{

		}

		~FXAARenderPass()
		{

		}

		inline void Initialize(int index)
		{
			activeTextureIndex = index;

			glGenFramebuffers(1, &frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

			glGenTextures(1, &textureBuffer);
			glBindTexture(GL_TEXTURE_2D, textureBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen::Width(), Screen::Height(), 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			_fxaaShader = Assets::LoadInternal<ShaderAsset>("data/shaders/ss_fxaa.shader").GetShader();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		inline void Render(const IRenderPass& lastPass, Camera& camera)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, lastPass.textureBuffer);

			_fxaaShader->Use();
			_fxaaShader->SetInt("screenTexture", 0);

			Internal::Quad::Render();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	};
}
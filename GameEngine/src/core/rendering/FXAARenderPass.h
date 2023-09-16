#pragma once

#include "IRenderPass.hpp"
#include <glad/glad.h>
#include "../Screen.h"
#include <cstddef>
#include "../Camera.hpp"
#include "../shaders/Shader.h"
#include "../assets/ShaderAsset.hpp"
#include "../Quad.hpp"

class FXAARenderPass : public IRenderPass
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

	void Initialize(int index)
	{
		activeTextureIndex = index;

		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenTextures(1, &textureBuffer);
		glBindTexture(GL_TEXTURE_2D, textureBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen::WIDTH, Screen::HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		_fxaaShader = Assets::Load<ShaderAsset>("shaders/ss_fxaa.shader").GetShader();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Render(const IRenderPass& lastPass, Camera& camera)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lastPass.textureBuffer);

		_fxaaShader->Use();
		//_fxaaShader->SetVector2("screenResolution", )
		_fxaaShader->SetInt("screenTexture", 0);

		Quad::Render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

};
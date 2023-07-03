#pragma once

#include "IRenderPass.hpp"
#include <glad/glad.h>
#include "../Screen.h"
#include <cstddef>
#include "../Camera.hpp"
#include "../shaders/Shader.h"
#include "../assets/ShaderAsset.hpp"


const float moreObnoxiousQuad[] = {
	-1.0f, -1.0f, -0.9f, 0.0f, 0.0f,
	1.0f, -1.0f, -0.9f, 1.0f, 0.0f,
	1.0f, 1.0f, -0.9f, 1.0f, 1.0f,
	-1.0f, -1.0f, -0.9f, 0.0f, 0.0f,
	1.0f, 1.0f, -0.9f, 1.0f, 1.0f,
	-1.0f, 1.0f, -0.9f, 0.0f, 1.0f
};

class BloomRenderPass : public IRenderPass
{
private:
	const IRenderPass& _scenePass;
	Shader* _bloomShader;
	unsigned int moreObnoxiousQuadVAO;

public:

	BloomRenderPass(const IRenderPass& scenePass) : _scenePass(scenePass)
	{

	}

	~BloomRenderPass()
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

		_bloomShader = Assets::Load<ShaderAsset>("shaders/ss_bloom.shader").GetShader();


		glGenVertexArrays(1, &moreObnoxiousQuadVAO);
		unsigned int moreObnoxiousQuadVBO;
		glGenBuffers(1, &moreObnoxiousQuadVBO);
		glBindVertexArray(moreObnoxiousQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, moreObnoxiousQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(moreObnoxiousQuad), moreObnoxiousQuad, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Render(Camera& camera)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _scenePass.textureBuffer);

		_bloomShader->Use();
		_bloomShader->SetInt("screenTexture", 0);

		glBindVertexArray(moreObnoxiousQuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

};
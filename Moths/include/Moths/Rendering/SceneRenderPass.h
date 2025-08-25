#pragma once

#include <glad/glad.h>
#include <cstddef>
#include <iostream>

#include <Moths/Core/Screen.h>
#include <Moths/Rendering/IRenderPass.h>
#include <Moths/Rendering/Renderers.h>

namespace MothsEngine::Internal
{

	class SceneRenderPass : public IRenderPass
	{
	private:
		GLuint multisampleTextureBuffer;
		GLuint multisampleFrameBuffer;

	public:

		const int MULTISAMPLES = 4;

		~SceneRenderPass()
		{

		}

		inline void Initialize(int index)
		{
			activeTextureIndex = index;

			//create a screen framebuffer

			glGenFramebuffers(1, &frameBuffer);

			glGenTextures(1, &textureBuffer);
			glBindTexture(GL_TEXTURE_2D, textureBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen::Width(), Screen::Height(), 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//attach to frame buffer

			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// configure MSAA framebuffer

			glGenFramebuffers(1, &multisampleFrameBuffer);

			//create a multisampled color attachment texture
			glGenTextures(1, &multisampleTextureBuffer);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleTextureBuffer);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTISAMPLES, GL_RGB, Screen::Width(), Screen::Height(), GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

			//create multisampled renderbuffer object for depth and stencil attachments

			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLES, GL_DEPTH24_STENCIL8, Screen::Width(), Screen::Height());
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			//attach to framebuffer

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, multisampleFrameBuffer);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampleTextureBuffer, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Multisampled Framebuffer is not complete!" << std::endl;

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		inline void Render(const IRenderPass& lastPass, Camera& camera)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, multisampleFrameBuffer);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_MULTISAMPLE);
			//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			//glEnable(GL_SAMPLE_ALPHA_TO_ONE);
			//glEnable(GL_SAMPLE_COVERAGE);

			for (auto* renderer : Renderers::GetRenderers())
			{
				renderer->Render(camera);
			}

			glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFrameBuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
			glBlitFramebuffer(0, 0, Screen::Width(), Screen::Height(), 0, 0, Screen::Width(), Screen::Height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);


			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

	};
}
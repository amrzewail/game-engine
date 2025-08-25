#pragma once
#include <glad/glad.h>

namespace MothsEngine::Internal
{
	const float moreObnoxiousQuad[] = {
	-1.0f, -1.0f, -0.9f, 0.0f, 0.0f,
	1.0f, -1.0f, -0.9f, 1.0f, 0.0f,
	1.0f, 1.0f, -0.9f, 1.0f, 1.0f,
	-1.0f, -1.0f, -0.9f, 0.0f, 0.0f,
	1.0f, 1.0f, -0.9f, 1.0f, 1.0f,
	-1.0f, 1.0f, -0.9f, 0.0f, 1.0f
	};

	class Quad
	{
	public:
		static GLuint VAO;

		static void Initialize()
		{
			glGenVertexArrays(1, &VAO);
			unsigned int moreObnoxiousQuadVBO;
			glGenBuffers(1, &moreObnoxiousQuadVBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VAO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(moreObnoxiousQuad), moreObnoxiousQuad, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		static void Render()
		{
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	};
}
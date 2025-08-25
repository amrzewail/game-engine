#include <Moths/Moths.h>
#include <Moths/Callbacks.h>
#include <Moths/Core/Utilities.h>

#include <Moths/Rendering/IRenderPass.h>
#include <Moths/Rendering/SceneRenderPass.h>
#include <Moths/Rendering/FXAARenderPass.h>
#include <Moths/Rendering/BloomRenderPass.h>

using namespace MothsEngine;
using namespace MothsEngine::Internal;

GLFWwindow* Moths::Window;

const char* screenShaderRaw = R"(vert
{
	#version 330 core
	layout (location = 0) in vec2 aPos;
	layout (location = 1) in vec2 aTexCoords;

	out vec2 TexCoords;

	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
		TexCoords = aTexCoords;
	}  
}

frag
{
	#version 330 core
	out vec4 FragColor;
  
	in vec2 TexCoords;

	uniform sampler2D screenTexture;

	void main()
	{ 
		vec4 color = texture(screenTexture, TexCoords);

		FragColor = color;

	}
})";

void Moths::Initialize()
{
	std::cout << "Moths Engine Initialized" << std::endl;

	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	Window = glfwCreateWindow(Screen::Width(), Screen::Height(), "Game", nullptr, nullptr);

	glfwMakeContextCurrent(Window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetWindowCloseCallback(Window, glfw_window_close_callback);

	glfwSetFramebufferSizeCallback(Window, glfw_framebuffer_size_callback);

	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	Internal::Quad::Initialize();

}

void Moths::Run()
{
	IRenderPass* sceneRenderPass = new SceneRenderPass();
	IRenderPass* bloomRenderPass = new BloomRenderPass();
	IRenderPass* fxaaRenderPass = new FXAARenderPass();

	std::vector<IRenderPass*> renderPasses;
	renderPasses.push_back(sceneRenderPass);
	renderPasses.push_back(bloomRenderPass);
	renderPasses.push_back(fxaaRenderPass);

	int index = 0;
	for (auto* pass : renderPasses)
	{
		pass->Initialize(index++);
	}

	int fpsCount = 0;
	const float startTime = glfwGetTime();
	float totalFrameTime = 0.0f;
	int frameMeasurementCount = 0;
	const int framesPerLog = 100;

	float deltaTime = 0.0f;
	float lastTimeSecs = 0.0f;

	float deltaTimeAccumulator = 0;
	const float targetFramerate = 60.0;
	const float targetFrameTime = 1.0 / targetFramerate;

	const Shader& screenShader = *Internal::ShaderCompiler::Compile(screenShaderRaw);

	while (!glfwWindowShouldClose(Window))
	{
		float currentTimeSecs = glfwGetTime();
		deltaTimeAccumulator += lastTimeSecs == 0.0f ? 0.0f : currentTimeSecs - lastTimeSecs;
		lastTimeSecs = currentTimeSecs;

		while (deltaTimeAccumulator >= targetFrameTime)
		{
			//update
			{
				deltaTime = deltaTimeAccumulator;
				deltaTimeAccumulator = 0;


				if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				{
					glfwSetWindowShouldClose(Window, true);
				}
			}

			//rendering
			{
				Camera* camera = Camera::CurrentCamera();
				if (camera)
				{
					camera->CalculateProjectionMatrix();

					IRenderPass* lastPass = renderPasses[0];
					for (auto* pass : renderPasses)
					{
						pass->Render(*lastPass, *camera);
						lastPass = pass;
					}


					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glClearColor(0, 0, 0, 0);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, lastPass->textureBuffer);

					screenShader.Use();
					screenShader.SetInt("screenTexture", 0);

					Quad::Render();
				}

			}

			glfwSwapBuffers(Window);
			glfwPollEvents();

			//std::cout << "average fps: " << 1.0f / deltaTime << " frame time: " << deltaTime << std::endl;
		}
	}

	Assets::UnloadAll();

	glfwDestroyWindow(Window);
	glfwTerminate();
}
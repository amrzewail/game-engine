#include <string>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cstdint>
#include <objbase.h>
#include <memory>
#include <unordered_map>
#include <chrono>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


#include "Utilities.h"
#include "Callbacks.h"
#include "DrawDetails.h"
#include "core/Vertex.h"
#include "OpenGLLoader.h"
#include "OpenGLDraw.h"
#include "Input.h"
#include "GLSLShaderLoader.h"

#include "core/Vector.h"
#include "core/Vector2.h"
#include "core/Vertex.h"
#include "core/Mesh.hpp"

#include "core/assets/Asset.h"
#include "core/assets/Assets.hpp"

#include "core/assets/TextAsset.hpp"
#include "core/assets/ModelAsset.hpp"
#include "core/assets/TextureAsset.hpp"

#include "core/shaders/Shader.h"
#include "core/Matrix4x4.hpp"
#include "core/Quaternion.hpp"
#include "core/Camera.hpp"
#include "core/Material.hpp"
#include "core/MeshRenderer.hpp"
#include "core/GameObject.hpp"
#include "core/lights/DirectionalLight.hpp"
#include "core/rendering/Renderers.hpp"
#include "core/assets/ShaderAsset.hpp"
#include "core/Screen.h"
#include "core/rendering/IRenderPass.hpp"
#include "core/rendering/SceneRenderPass.h"
#include "core/rendering/BloomRenderPass.hpp"
#include "core/rendering/FXAARenderPass.h"
#include "core/Quad.hpp"

float deltaTime = 0.0f;
float lastTimeMs = 0.0f;


int main(int argc, char** argv)
{
	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(Screen::WIDTH, Screen::HEIGHT, ExtractVersion(argv[0]), nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetWindowCloseCallback(window, glfw_window_close_callback);

	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Quad::Initialize();

	const Shader& screenShader = *Assets::Load<ShaderAsset>("shaders/screen.shader").GetShader();

	const Shader& hairShader = *Assets::Load<ShaderAsset>("shaders/hair.shader").GetShader();
	const Shader& lit = *Assets::Load<ShaderAsset>("shaders/lit.shader").GetShader();
	const Shader& litClip = *Assets::Load<ShaderAsset>("shaders/lit_clip.shader").GetShader();
	const Shader& litTransparent = *Assets::Load<ShaderAsset>("shaders/lit_transparent.shader").GetShader();
	const Shader& eyeShader = *Assets::Load<ShaderAsset>("shaders/eye.shader").GetShader();
	const Shader& faceShader = *Assets::Load<ShaderAsset>("shaders/face.shader").GetShader();

	TextureAsset& gridTex = Assets::Load<TextureAsset>("textures/checkerboard_tex.png");
	TextureAsset& fabricTex = Assets::Load<TextureAsset>("textures/FABRIC_tex.png");

	Material fabricMaterial = Material("Fabric", lit, fabricTex.GetTexture());
	Material gridMaterial = Material("Grid", lit, gridTex.GetTexture());

	ModelAsset& jayneModel = Assets::Load<ModelAsset>("models/Jayne.fbx");

	ModelAsset& xbotModel = Assets::Load<ModelAsset>("models/xbot.fbx");

	ModelAsset& cube = Assets::Load<ModelAsset>("models/cube.obj");


	Camera& camera = *new Camera();
	camera.aspect = float(Screen::WIDTH) / Screen::HEIGHT;

	GameObject& jayneGameObject = *new GameObject();
	//jayne model loading and renderer
	{

		TextureAsset& faceTex = Assets::Load<TextureAsset>("textures/FACE_tex.png");
		TextureAsset& bodyTex = Assets::Load<TextureAsset>("textures/BODY_tex.png");
		TextureAsset& hairTex = Assets::Load<TextureAsset>("textures/HAIR_tex.png");
		TextureAsset& clothTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");
		TextureAsset& leatherTex = Assets::Load<TextureAsset>("textures/Leather_tex.png");
		TextureAsset& goldTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");
		TextureAsset& chainmailTex = Assets::Load<TextureAsset>("textures/CHAINMAIL_tex.jpg");
		TextureAsset& scarfTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");

		std::vector<Material*> jayneMaterials = std::vector<Material*>();

		Material& hairMaterial = *new Material("Hair", hairShader, hairTex.GetTexture());
		hairMaterial.SetColor("ucolor", Color(0.6, 0, 0, 1));

		jayneMaterials.push_back(new Material("Eyeball", eyeShader, faceTex.GetTexture()));
		jayneMaterials.push_back(new Material("Face", faceShader, faceTex.GetTexture()));
		jayneMaterials.push_back(new Material("Body", lit, bodyTex.GetTexture()));
		jayneMaterials.push_back(new Material("Eyelashes", litClip, faceTex.GetTexture()));
		jayneMaterials.push_back(&hairMaterial);
		jayneMaterials.push_back(new Material("Hair2", hairShader, hairTex.GetTexture()));
		jayneMaterials.push_back(new Material("Underwear", lit, clothTex.GetTexture()));
		jayneMaterials.push_back(new Material("Cloth", lit, clothTex.GetTexture()));
		jayneMaterials.push_back(new Material("Leather", lit, leatherTex.GetTexture()));
		jayneMaterials.push_back(new Material("Gold", lit, goldTex.GetTexture()));
		jayneMaterials.push_back(new Material("Chainmail", litClip, chainmailTex.GetTexture()));
		jayneMaterials.push_back(new Material("Scarf", lit, scarfTex.GetTexture()));
		jayneMaterials.push_back(new Material("Brown", lit, leatherTex.GetTexture()));


		for (int i = 0; i < jayneModel.Meshes().size(); i++)
		{
			//if (*jayneModel.Meshes()[i]->materialName != "Eyeball") continue;

			MeshRenderer& renderer = jayneGameObject.AddComponent<MeshRenderer>();
			renderer.mesh = jayneModel.Meshes()[i]->mesh;
			for (auto* material : jayneMaterials)
			{
				if (material->name == *jayneModel.Meshes()[i]->materialName)
				{
					renderer.material = material;
					break;
				}
			}
		}
	}

	GameObject& xbotGameObject = *new GameObject();
	{
		*xbotGameObject.transform->position = Vector(0, 0, 5);
		*xbotGameObject.transform->localScale = Vector(0.01, 0.01, 0.01);
		for (int i = 0; i < xbotModel.Meshes().size(); i++)
		{
			//if (*jayneModel.Meshes()[i]->materialName != "Eyeball") continue;

			MeshRenderer& renderer = xbotGameObject.AddComponent<MeshRenderer>();
			renderer.mesh = xbotModel.Meshes()[i]->mesh;
			renderer.material = &gridMaterial;
			//for (auto* material : jayneMaterials)
			//{
			//	if (material->name == *jayneModel.Meshes()[i]->materialName)
			//	{
			//		renderer.material = material;
			//		break;
			//	}
			//}
		}
	}


	GameObject& floorGameObject = *new GameObject();
	{// floor game object renderer


		Material* gridMaterial = new Material("Grid", lit, fabricTex.GetTexture());

		MeshRenderer& renderer = floorGameObject.AddComponent<MeshRenderer>();
		renderer.material = gridMaterial;
		renderer.mesh = cube.Meshes()[0]->mesh;
	}


	DirectionalLight* directionalLight = new DirectionalLight();
	directionalLight->intensity = 1.5;
	directionalLight->direction = new Vector(0, -1, -1);
	directionalLight->color = new Color(1, 0.8, 0.8, 1);

	float rotY = 0;

	int fpsCount = 0;
	const float startTime = glfwGetTime();
	float totalFrameTime = 0.0f;
	int frameMeasurementCount = 0;
	const int framesPerLog = 100;

	float deltaTimeAccumulator = 0;
	const float targetFramerate = 60.0;
	const float targetFrameTime = 1.0 / targetFramerate;


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

	*camera.transform->position = Vector(0.15, 1.6, -0.3);
	*camera.transform->rotation = Quaternion::Euler(-10, 30, 0);

	double lastMouseX = 0, lastMouseY = 0;

	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	//glEnable(GL_SAMPLE_ALPHA_TO_ONE);
	//glEnable(GL_SAMPLE_COVERAGE);

	while (!glfwWindowShouldClose(window))
	{
		float currentTimeMs = glfwGetTime();
		deltaTimeAccumulator += lastTimeMs == 0.0f ? 0.0f : currentTimeMs - lastTimeMs;
		lastTimeMs = currentTimeMs;

		while (deltaTimeAccumulator >= targetFrameTime)
		{
			//update
			{
				deltaTime = deltaTimeAccumulator;
				deltaTimeAccumulator = 0;



				rotY += 4 * deltaTime;

				*directionalLight->direction = Vector(-0.6, -0.6, 1);

				camera.CalculateProjectionMatrix();

				//inputs
				{
					float cameraSpeed = 2;

					if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
					{
						*camera.transform->position += camera.transform->Right() * cameraSpeed * deltaTime;
					}

					if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
					{
						*camera.transform->position += -camera.transform->Right() * cameraSpeed * deltaTime;
					}

					if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
					{
						*camera.transform->position += camera.transform->Forward() * cameraSpeed * deltaTime;
					}

					if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
					{
						*camera.transform->position += -camera.transform->Forward() * cameraSpeed * deltaTime;
					}

					if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
					{
						camera.transform->position->y += cameraSpeed * deltaTime;
					}

					if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
					{
						camera.transform->position->y -= cameraSpeed * deltaTime;
					}

					double mouseX, mouseY;
					glfwGetCursorPos(window, &mouseX, &mouseY);

					Vector cameraEuler = camera.transform->EulerAngles();

					cameraEuler.y -= (mouseX - lastMouseX) * deltaTime;
					cameraEuler.x -= (mouseY - lastMouseY) * deltaTime;
					cameraEuler.z = 0;

					lastMouseX = mouseX;
					lastMouseY = mouseY;

					*camera.transform->rotation = cameraEuler;

					//std::cout << "Camera Position: " << camera.transform->position->ToString() << std::endl;

					ProcessInput(window);

				}

				{
					floorGameObject.transform->position->y = -0.1;
					floorGameObject.transform->localScale = new Vector(2, 0.01, 2);
				}


				{
					*jayneGameObject.transform->position = Vector(0, 0, 0);
					*jayneGameObject.transform->rotation = Quaternion::Euler(270, 0, 0);
				}
			}

			//rendering
			{
				IRenderPass* lastPass = renderPasses[0];
				for (auto* pass : renderPasses)
				{
					pass->Render(*lastPass, camera);
					lastPass = pass;
				}


				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClearColor(0, 0, 0, 0);
				glClear(GL_COLOR_BUFFER_BIT);
				glDisable(GL_DEPTH_TEST);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, lastPass->textureBuffer);

				screenShader.Use();
				screenShader.SetInt("screenTexture", 0);

				Quad::Render();
			}

			glfwSwapBuffers(window);
			glfwPollEvents();

			//std::cout << "average fps: " << 1.0f / deltaTime << " frame time: " << deltaTime << std::endl;
		}
	}


	delete& jayneGameObject;

	Assets::UnloadAll();


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
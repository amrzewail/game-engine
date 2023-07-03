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



float deltaTime = 0.0f;
float lastTimeMs = 0.0f;

int main(int argc, char** argv)
{
	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(1280, 720, ExtractVersion(argv[0]), nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetWindowCloseCallback(window, glfw_window_close_callback);

	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

	const Shader& lit = *Assets::Load<ShaderAsset>("shaders/lit.shader").GetShader();
	const Shader& litClip = *Assets::Load<ShaderAsset>("shaders/lit_clip.shader").GetShader();
	const Shader& litTransparent = *Assets::Load<ShaderAsset>("shaders/lit_transparent.shader").GetShader();
	const Shader& eyeShader = *Assets::Load<ShaderAsset>("shaders/eye.shader").GetShader();

	TextureAsset& faceTex = Assets::Load<TextureAsset>("textures/FACE_tex.png");
	TextureAsset& bodyTex = Assets::Load<TextureAsset>("textures/BODY_tex.png");
	TextureAsset& hairTex = Assets::Load<TextureAsset>("textures/HAIR_tex.png");
	TextureAsset& clothTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");
	TextureAsset& leatherTex = Assets::Load<TextureAsset>("textures/Leather_tex.png");
	TextureAsset& goldTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");
	TextureAsset& chainmailTex = Assets::Load<TextureAsset>("textures/CHAINMAIL_tex.jpg");
	TextureAsset& scarfTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");

	TextureAsset& gridTex = Assets::Load<TextureAsset>("textures/checkerboard_tex.png");
	TextureAsset& fabricTex = Assets::Load<TextureAsset>("textures/FABRIC_tex.png");

	Material fabricMaterial = Material("Fabric", lit, fabricTex.GetTexture());

	ModelAsset& jayneModel = Assets::Load<ModelAsset>("models/Jayne.fbx");

	ModelAsset& cube = Assets::Load<ModelAsset>("models/cube.obj");


	Camera& camera = *new Camera();
	camera.aspect = 1280.0 / 720.0;

	GameObject& jayneGameObject = *new GameObject();
	//jayne model loading and renderer
	{
		std::vector<Material*> jayneMaterials = std::vector<Material*>();

		jayneMaterials.push_back(new Material("Eyeball", eyeShader, faceTex.GetTexture()));
		jayneMaterials.push_back(new Material("Face", lit, faceTex.GetTexture()));
		jayneMaterials.push_back(new Material("Body", lit, bodyTex.GetTexture()));
		jayneMaterials.push_back(new Material("Eyelashes", litClip, faceTex.GetTexture()));
		jayneMaterials.push_back(new Material("Hair", litTransparent, hairTex.GetTexture()));
		jayneMaterials.push_back(new Material("Hair2", litClip, hairTex.GetTexture()));
		jayneMaterials.push_back(new Material("Underwear", lit, clothTex.GetTexture()));
		jayneMaterials.push_back(new Material("Cloth", lit, clothTex.GetTexture()));
		jayneMaterials.push_back(new Material("Leather", lit, leatherTex.GetTexture()));
		jayneMaterials.push_back(new Material("Gold", lit, goldTex.GetTexture()));
		jayneMaterials.push_back(new Material("Chainmail", litClip, chainmailTex.GetTexture()));
		jayneMaterials.push_back(new Material("Scarf", lit, scarfTex.GetTexture()));
		jayneMaterials.push_back(new Material("Brown", lit, leatherTex.GetTexture()));


		for (int i = 0; i < jayneModel.Meshes().size(); i++)
		{
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


	GameObject& floorGameObject = *new GameObject();
	{// floor game object renderer


		Material* gridMaterial = new Material("Grid", lit, fabricTex.GetTexture());

		MeshRenderer& renderer = floorGameObject.AddComponent<MeshRenderer>();
		renderer.material = gridMaterial;
		renderer.mesh = cube.Meshes()[0]->mesh;
	}


	DirectionalLight* directionalLight = new DirectionalLight();
	directionalLight->intensity = 1;
	directionalLight->direction = new Vector(0, -1, -1);
	directionalLight->color = new Color(1, 1, 1, 1);

	float rotY = 0;

	int fpsCount = 0;
	const float startTime = glfwGetTime();
	float totalFrameTime = 0.0f;
	int frameMeasurementCount = 0;
	const int framesPerLog = 100;

	float deltaTimeAccumulator = 0;
	const float targetFramerate = 60.0;
	const float targetFrameTime = 1.0 / targetFramerate;


	glEnable(GL_TEXTURE_DEPTH);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 0);

	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_MULTISAMPLE);

	while (!glfwWindowShouldClose(window))
	{
		float currentTimeMs = glfwGetTime();
		deltaTimeAccumulator += lastTimeMs == 0.0f ? 0.0f : currentTimeMs - lastTimeMs;
		lastTimeMs = currentTimeMs;

		while (deltaTimeAccumulator >= targetFrameTime)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glDisable(GL_BLEND);

			deltaTime = deltaTimeAccumulator;
			deltaTimeAccumulator = 0;

			camera.transform->position = new Vector(0.01, 1.6, -0.5);
			camera.transform->rotation = Quaternion::Euler(-10, 0, 0);

			rotY += 4 * deltaTime;


			glfwPollEvents();
			ProcessInput(window);

			camera.CalculateProjectionMatrix();

			{
				floorGameObject.transform->position->y = -0.1;
				floorGameObject.transform->localScale = new Vector(2, 0.01, 2);
			}


			{
				jayneGameObject.transform->rotation = Quaternion::Euler(270, 0 * rotY * 10, 0);
			}


			for (auto* renderer : Renderers::GetRenderers())
			{
				renderer->Render(camera);
			}

			glfwSwapBuffers(window);

			//std::cout << "average fps: " << 1.0f / deltaTime << " frame time: " << deltaTime << std::endl;
		}
	}


	delete& jayneGameObject;

	Assets::UnloadAll();


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
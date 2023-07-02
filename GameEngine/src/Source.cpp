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

	glClearColor(.2f, .2f, .6f, 0.f);

	const Shader& shader = Shader::Create("shaders/lit.vert", "shaders/lit.frag");
	const Shader& transparentShader = Shader::Create("shaders/lit.vert", "shaders/lit_transparent.frag");

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

	Material fabricMaterial = Material(shader, fabricTex.GetTexture());

	Camera& camera = *new Camera();
	camera.aspect = 1280.0 / 720.0;


	GameObject& jayneGameObject = *new GameObject();
	//jayne model loading and renderer
	{
		Material* eyeballMaterial = new Material(shader, faceTex.GetTexture());
		Material* faceMaterial = new Material(shader, faceTex.GetTexture());
		Material* bodyMaterial = new Material(shader, bodyTex.GetTexture());
		Material* eyelashesMaterial = new Material(transparentShader, faceTex.GetTexture());
		Material* hairMaterial = new Material(shader, hairTex.GetTexture());
		Material* hair2Material = new Material(shader, hairTex.GetTexture());
		Material* underwearMaterial = new Material(shader, clothTex.GetTexture());
		Material* clothMaterial = new Material(shader, clothTex.GetTexture());
		Material* leatherMaterial = new Material(shader, leatherTex.GetTexture());
		Material* goldMaterial = new Material(shader, goldTex.GetTexture());
		Material* chainmailMaterial = new Material(transparentShader, chainmailTex.GetTexture());
		Material* scarfMaterial = new Material(shader, scarfTex.GetTexture());
		Material* brownMaterial = new Material(shader, leatherTex.GetTexture());

		chainmailMaterial->enableTransparency = true;
		hairMaterial->enableTransparency = true;
		hair2Material->enableTransparency = true;
		eyelashesMaterial->enableTransparency = true;

		chainmailMaterial->isTwoSided = true;
		hairMaterial->isTwoSided = true;
		hair2Material->isTwoSided = true;
		scarfMaterial->isTwoSided = true;

		std::unordered_map<std::string, Material*> materialMap = std::unordered_map<std::string, Material*>();
		materialMap["Eyeball"] = eyeballMaterial;
		materialMap["Face"] = faceMaterial;
		materialMap["Body"] = bodyMaterial;
		materialMap["Eyelashes"] = eyelashesMaterial;
		materialMap["Hair"] = hairMaterial;
		materialMap["Hair2"] = hair2Material;
		materialMap["Underwear"] = underwearMaterial;
		materialMap["Cloth"] = clothMaterial;
		materialMap["Leather"] = leatherMaterial;
		materialMap["Gold"] = goldMaterial;
		materialMap["Chainmail"] = chainmailMaterial;
		materialMap["Scarf"] = scarfMaterial;
		materialMap["Brown"] = brownMaterial;

		ModelAsset& jayneModel = Assets::Load<ModelAsset>("models/Jayne.fbx");

		for (int i = 0; i < jayneModel.Meshes().size(); i++)
		{
			//MeshRenderer* r = new MeshRenderer();
			//jayneGameObject.AddComponent(r);
			MeshRenderer& renderer = jayneGameObject.AddComponent<MeshRenderer>();
			renderer.mesh = jayneModel.Meshes()[i]->mesh;
			renderer.material = materialMap.at(*jayneModel.Meshes()[i]->materialName);
		}
	}

	GameObject& floorGameObject = *new GameObject();
	{// floor game object renderer

		ModelAsset& cube = Assets::Load<ModelAsset>("models/cube.obj");

		Material* gridMaterial = new Material(shader, fabricTex.GetTexture());

		MeshRenderer& renderer = floorGameObject.AddComponent<MeshRenderer>();
		renderer.material = gridMaterial;
		renderer.mesh = cube.Meshes()[0]->mesh;
	}

	//GameObject& floorGameObject2 = *new GameObject();
	//{// floor game object renderer

	//	ModelAsset& cube = Assets::Load<ModelAsset>("models/cube.obj");

	//	Material* gridMaterial = new Material(shader, fabricTex.GetTexture());

	//	MeshRenderer& renderer = floorGameObject2.AddComponent<MeshRenderer>();
	//	renderer.material = gridMaterial;
	//	renderer.mesh = cube.Meshes()[0]->mesh;
	//}

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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	while (!glfwWindowShouldClose(window))
	{
		float currentTimeMs = glfwGetTime();
		deltaTimeAccumulator += lastTimeMs == 0.0f ? 0.0f : currentTimeMs - lastTimeMs;
		lastTimeMs = currentTimeMs;

		while (deltaTimeAccumulator >= targetFrameTime)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			deltaTime = deltaTimeAccumulator;
			deltaTimeAccumulator = 0;

			camera.transform->position = new Vector(0.01, 1.6, -1);
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
				jayneGameObject.transform->rotation = Quaternion::Euler(270, rotY * 10, 0);
			}


			for (auto* renderer : Renderers::GetRenderers())
			{
				renderer->Render(camera);

				glDisable(GL_BLEND);
				glEnable(GL_CULL_FACE);
			}

			glfwSwapBuffers(window);

			//std::cout << "average fps: " << 1.0f / deltaTime << " frame time: " << deltaTime << std::endl;
		}
	}

	delete &jayneGameObject;


	Assets::Unload(fabricTex.path);
	Assets::Unload(gridTex.path);

	Assets::Unload(faceTex.path);
	Assets::Unload(bodyTex.path);
	Assets::Unload(faceTex.path);
	Assets::Unload(hairTex.path);
	Assets::Unload(clothTex.path);
	Assets::Unload(leatherTex.path);
	Assets::Unload(goldTex.path);
	Assets::Unload(chainmailTex.path);
	Assets::Unload(scarfTex.path);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
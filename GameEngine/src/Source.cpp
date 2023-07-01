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



float deltaTime = 0.0f;
float lastTimeMs = 0.0f;

int main(int argc, char** argv)
{


	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	GLFWwindow* window = glfwCreateWindow(1280, 720, ExtractVersion(argv[0]), nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetWindowCloseCallback(window, glfw_window_close_callback);

	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

	glClearColor(.2f, .2f, .6f, 0.f);

	const Shader& shader = Shader::Create("shaders/lit.vert", "shaders/lit.frag");
	const Shader& chainmailShader = Shader::Create("shaders/lit.vert", "shaders/chainmail.frag");

	TextureAsset& faceTex = Assets::Load<TextureAsset>("textures/FACE_tex.png");
	TextureAsset& bodyTex = Assets::Load<TextureAsset>("textures/BODY_tex.png");
	TextureAsset& hairTex = Assets::Load<TextureAsset>("textures/HAIR_tex.png");
	TextureAsset& clothTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");
	TextureAsset& leatherTex = Assets::Load<TextureAsset>("textures/Leather_tex.png");
	TextureAsset& goldTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");
	TextureAsset& chainmailTex = Assets::Load<TextureAsset>("textures/CHAINMAIL_tex.jpg");
	TextureAsset& scarfTex = Assets::Load<TextureAsset>("textures/CLOTH_tex.png");

	TextureAsset& fabricTex = Assets::Load<TextureAsset>("textures/FABRIC_tex.png");

	TextureAsset& gridTex = Assets::Load<TextureAsset>("textures/checkerboard_tex.png");

	Material fabricMaterial = Material(shader, fabricTex.Tex());

	Material gridMaterial = Material(shader, gridTex.Tex());

	Assets::Unload(fabricTex.path);

	ModelAsset& cube = Assets::Load<ModelAsset>("models/cube.obj");

	Camera& camera = *new Camera();
	camera.aspect = 1280.0 / 720.0;

	std::vector<MeshRenderer*> jayneMeshRenderers = std::vector<MeshRenderer*>();
	//jayne model loading and renderer
	{
		Material* eyeballMaterial = new Material(shader, faceTex.Tex());
		Material* faceMaterial = new Material(shader, faceTex.Tex());
		Material* bodyMaterial = new Material(shader, bodyTex.Tex());
		Material* eyelashesMaterial = new Material(chainmailShader, faceTex.Tex());
		Material* hairMaterial = new Material(shader, hairTex.Tex());
		Material* hair2Material = new Material(shader, hairTex.Tex());
		Material* underwearMaterial = new Material(shader, clothTex.Tex());
		Material* clothMaterial = new Material(shader, clothTex.Tex());
		Material* leatherMaterial = new Material(shader, leatherTex.Tex());
		Material* goldMaterial = new Material(shader, goldTex.Tex());
		Material* chainmailMaterial = new Material(chainmailShader, chainmailTex.Tex());
		Material* scarfMaterial = new Material(shader, scarfTex.Tex());
		Material* brownMaterial = new Material(shader, leatherTex.Tex());

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
			MeshRenderer* renderer = new MeshRenderer();
			renderer->mesh = jayneModel.Meshes()[i]->mesh;
			renderer->materials.push_back(materialMap.at(*jayneModel.Meshes()[i]->materialName));

			jayneMeshRenderers.push_back(renderer);
		}


		Assets::Unload(faceTex.path);
		Assets::Unload(bodyTex.path);
		Assets::Unload(faceTex.path);
		Assets::Unload(hairTex.path);
		Assets::Unload(clothTex.path);
		Assets::Unload(leatherTex.path);
		Assets::Unload(goldTex.path);
		Assets::Unload(chainmailTex.path);
		Assets::Unload(scarfTex.path);
	}


	float rotY = 0;

	int fpsCount = 0;
	const float startTime = glfwGetTime();
	float totalFrameTime = 0.0f;
	int frameMeasurementCount = 0;
	const int framesPerLog = 100;

	float deltaTimeAccumulator = 0;
	const float targetFramerate = 60.0;
	const float targetFrameTime = 1.0 / targetFramerate;


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_DEPTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

			camera.transform->position = Vector(0.01, 2, -2 + sin(rotY / 15) * 1);
			camera.transform->rotation = *Quaternion::Euler(-30 + 0 * 40 * sin(rotY / 10), 0, 0);

			rotY += 4 * deltaTime;


			glfwPollEvents();
			ProcessInput(window);


			const Matrix4x4 PV = camera.ProjectionMatrix();

			const Matrix4x4 cubeModel = *Matrix4x4::TRS(*new Vector(0, -0.1, 0), *Quaternion::Euler(0, 0, 0), *new Vector(2, 0.01, 2));
			const Matrix4x4 jayneModel = *Matrix4x4::TRS(*new Vector(0, 0, 0), *Quaternion::Euler(270, rotY * 10, 0), *new Vector(1, 1, 1));

			const Matrix4x4 cubeRightModel = *Matrix4x4::TRS(*new Vector(1, 0, 0), *Quaternion::Euler(0, 0, 0), *new Vector(0.05, 0.05, 0.05));


			{
				fabricMaterial.Use();
				fabricMaterial.Activate();

				fabricMaterial.shader.SetMatrix4x4("model", cubeRightModel);
				fabricMaterial.shader.SetMatrix4x4("PV", PV);

				for (int i = 0; i < cube.Meshes().size(); i++)
				{
					cube.Meshes()[i]->mesh->Draw();
				}
			}

			{
				gridMaterial.Use();
				gridMaterial.Activate();

				gridMaterial.shader.SetMatrix4x4("model", cubeModel);
				gridMaterial.shader.SetMatrix4x4("PV", PV);

				for (int i = 0; i < cube.Meshes().size(); i++)
				{
					cube.Meshes()[i]->mesh->Draw();
				}
			}

			{
				for (int i = 0; i < jayneMeshRenderers.size(); i++)
				{
					jayneMeshRenderers[i]->materials[0]->Use();
					jayneMeshRenderers[i]->materials[0]->Activate();

					jayneMeshRenderers[i]->materials[0]->shader.SetMatrix4x4("model", jayneModel);
					jayneMeshRenderers[i]->materials[0]->shader.SetMatrix4x4("PV", PV);

					jayneMeshRenderers[i]->Render();
				}
			}

			glUseProgram(0);
			glfwSwapBuffers(window);
			glfwPollEvents();

			std::cout << "average fps: " << 1.0f / deltaTime << " frame time: " << deltaTime << std::endl;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
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
#include "Input.h"
#include "GLSLShaderLoader.h"

#include "core/Vector.h"
#include "core/Vector2.h"
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
#include "core/events/Event.h"

float deltaTime = 0.0f;
float lastTimeSecs = 0.0f;

//{
//	int number = 0;
//	int step = 90;
//	for (int x = 0; x <= 360; x += step)
//	{
//		for (int y = 0; y <= 360; y += step)
//		{
//			for (int z = 0; z <= 360; z += step)
//			{
//				Vector euler(x, y, z);
//				Quaternion q = Quaternion::Euler(euler.x, euler.y, euler.z);
//				Vector v = q.EulerAngles();
//				std::cout << number++ << ") " << euler.ToString() << " " << q.ToString() << " " << v.ToString() << std::endl << std::endl;
//			}
//		}
//	}
//	//Quaternion q = Quaternion(0.5, 0, 0.86603, 0);
//	//Quaternion q(0.5, 0, 0.866, 0);
//	//Quaternion q = Quaternion::Euler(89,179, 0);
//	//std::cout << q.ToString() << std::endl;
//	//std::cout << q.EulerAngles().ToString() << std::endl;
//	//float angle = Vector::Angle(Vector(0, 1, 1), Vector(0, 1, 0));
//	//std::cout << angle << std::endl;
//}


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

	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

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

	ModelAsset& jayneModel = Assets::Load<ModelAsset>("models/Jayne_simple.fbx");
	//ModelAsset& cube = Assets::Load<ModelAsset>("models/cube.obj");

	std::cout << jayneModel.GetModel()->armature->ToString() << std::endl;

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

		std::map<std::string, Material*> jayneMaterials = std::map<std::string, Material*>();

		Material& hairMaterial = *new Material("Hair", hairShader, hairTex.GetTexture());
		hairMaterial.SetColor("ucolor", Color(0.6, 0, 0, 1));

		jayneMaterials["Adventurer_Pants"] = new Material("Adventurer_Pants", lit, clothTex.GetTexture());
		jayneMaterials["Adventurer_Shirt"] = new Material("Adventurer_Shirt", lit, clothTex.GetTexture());

		jayneMaterials["Body"] = new Material("Body", lit, bodyTex.GetTexture());
		jayneMaterials["Body_Chest"] = new Material("Body_Chest", lit, bodyTex.GetTexture());
		jayneMaterials["Body_Shorts"] = new Material("Body_Shorts", lit, bodyTex.GetTexture());

		jayneMaterials["Cloth"] = new Material("Cloth", lit, clothTex.GetTexture());

		jayneMaterials["Eyeball"] = new Material("Eyeball", eyeShader, faceTex.GetTexture());
		jayneMaterials["Eyelashes"] = new Material("Eyelashes", litClip, faceTex.GetTexture());

		jayneMaterials["Face"] = new Material("Face", faceShader, faceTex.GetTexture());

		jayneMaterials["Gold"] = new Material("Gold", lit, goldTex.GetTexture());

		jayneMaterials["Hair"] = &hairMaterial;

		jayneMaterials["Leather"] = new Material("Leather", lit, leatherTex.GetTexture());

		int jayneMeshSize = jayneModel.GetModel()->meshes.size();
		for (int i = jayneMeshSize - 1; i >= 0; i--)
		{
			auto mesh = jayneModel.GetModel()->meshes[i];
			MeshRenderer& renderer = jayneGameObject.AddComponent<MeshRenderer>();
			renderer.SetMesh(mesh);
			renderer.animation = *jayneModel.GetModel()->animations[0];
			renderer.armature = jayneModel.GetModel()->armature;

			auto meshMaterials = mesh->GetMaterials();

			for (auto meshMaterial : meshMaterials)
			{
				renderer.materials.push_back(jayneMaterials[meshMaterial]);
			}
		}
	}

	//GameObject& floorGameObject = *new GameObject();
	//{// floor game object renderer
	//	Material* gridMaterial = new Material("Grid", lit, gridTex.GetTexture());

	//	MeshRenderer& renderer = floorGameObject.AddComponent<MeshRenderer>();
	//	renderer.materials.push_back(gridMaterial);
	//	renderer.SetMesh(cube.GetModel()->meshes[0]);
	//}


	//{
	//	floorGameObject.transform->position = Vector(0, -0.1, 0);
	//	floorGameObject.transform->localScale = Vector(2, 0.01, 2);
	//}


	DirectionalLight* directionalLight = new DirectionalLight();
	directionalLight->intensity = 1.5;
	directionalLight->direction = Vector(0, -1, 1);
	directionalLight->color = Color(1, 0.8, 0.8, 1);

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

	double lastMouseX = 0, lastMouseY = 0;

	//camera
	{
		camera.transform->position = Vector(0, 3, -3);
		camera.transform->set_EulerAngles(Vector(30, 0, 0));
	}

	//jayne
	{
		jayneGameObject.transform->position = Vector::Zero();
		//jayneGameObject.transform->set_EulerAngles(0, 180, 0);
	}

	while (!glfwWindowShouldClose(window))
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

				rotY += 1 * deltaTime;

				camera.CalculateProjectionMatrix();

				//inputs
				{
					//Camera controls
					{
						float cameraSpeed = 2;

						double mouseX, mouseY;
						glfwGetCursorPos(window, &mouseX, &mouseY);

						Vector cameraEuler = camera.transform->get_EulerAngles();

						cameraEuler.y += (mouseX - lastMouseX) * deltaTime;
						cameraEuler.x += (mouseY - lastMouseY) * deltaTime;
						cameraEuler.z = 0;

						lastMouseX = mouseX;
						lastMouseY = mouseY;

						camera.transform->set_EulerAngles(cameraEuler);

						Vector direction = Vector::Forward();
						direction = camera.transform->get_Rotation() * direction;
						camera.transform->position = (jayneGameObject.transform->position + Vector::Up()) - direction * 2;

					}



					//jayne inputs
					{

						Vector euler = jayneGameObject.transform->get_EulerAngles();
						Vector position = jayneGameObject.transform->position;

						Vector cameraForward = camera.transform->Forward();
						cameraForward.y = 0;
						cameraForward.Normalize();

						Vector cameraRight = camera.transform->Right();
						cameraRight.y = 0;
						cameraRight.Normalize();


						if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
						{
							position += cameraRight * deltaTime;
							euler.y = camera.transform->get_EulerAngles().y + 90;
						}

						if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
						{
							position -= cameraRight * deltaTime;
							euler.y = camera.transform->get_EulerAngles().y - 90;
						}

						if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
						{
							position += cameraForward * deltaTime;
							euler.y = camera.transform->get_EulerAngles().y;
						}

						if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
						{
							position -= cameraForward * deltaTime;
							euler.y = camera.transform->get_EulerAngles().y + 180;
						}

						jayneGameObject.transform->position = position;
						jayneGameObject.transform->set_EulerAngles(euler);


						euler = jayneGameObject.transform->get_EulerAngles();
						//std::cout << "forward: " + jayneGameObject.transform->Forward().ToString() + " position:" + position.ToString() + " euler:" + euler.ToString() << std::endl;
					}

					ProcessInput(window);
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
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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


	Assets::UnloadAll();


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}



/*

					//Camera controls
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

						Vector cameraEuler = camera.transform->get_EulerAngles();

						//std::cout << "mouseX: " << (mouseX - lastMouseX) << "  mouseY: " << -(mouseY - lastMouseY) << std::endl;

						cameraEuler.y += (mouseX - lastMouseX) * deltaTime;
						cameraEuler.x += (mouseY - lastMouseY) * deltaTime;
						cameraEuler.z = 0;

						lastMouseX = mouseX;
						lastMouseY = mouseY;

						//std::cout << cameraEuler.ToString() << std::endl;

						camera.transform->set_EulerAngles(cameraEuler);

						std::cout << camera.transform->Forward().ToString() << std::endl;
					}


*/
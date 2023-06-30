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

#include "core/shaders/Shader.h"


int main(int argc, char** argv)
{
	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	GLFWwindow* window = glfwCreateWindow(800, 600, ExtractVersion(argv[0]), nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetWindowCloseCallback(window, glfw_window_close_callback);

	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

	glClearColor(.2f, .2f, .6f, 0.f);

	const Shader& shader = Shader::Create("shaders/lit.vert", "shaders/lit.frag");



	//std::vector<DrawDetails> ourDrawDetails;
	//{
	//	std::vector<Vertex> obj_pts;

	//	Vertex v1;
	//	v1.position = Vector(.5f, -.5f, 0.f);
	//	obj_pts.push_back(v1);

	//	Vertex v2;
	//	v2.position = Vector(-.5f, -.5f, 0.f);
	//	obj_pts.push_back(v2);

	//	Vertex v3;
	//	v3.position = Vector(0.f, .5f, 0.f);
	//	obj_pts.push_back(v3);

	//	std::vector<uint32_t> elem = { 0, 1, 2 };

	//	UploadMesh(obj_pts, elem);
	//};

	//shader.Use();

	ModelAsset& model = Assets::Load<ModelAsset>("models/Jayne.fbx");

	//std::vector<Vertex> obj_pts;

	//Vertex v1;
	//v1.position = Vector(.5f, -.5f, 0.f);
	//obj_pts.push_back(v1);

	//Vertex v2;
	//v2.position = Vector(-.5f, -.5f, 0.f);
	//obj_pts.push_back(v2);

	//Vertex v3;
	//v3.position = Vector(0.f, .5f, 0.f);
	//obj_pts.push_back(v3);

	//std::vector<uint32_t> elem = { 0, 1, 2 };

	//Mesh mesh(obj_pts, elem);


	while (!glfwWindowShouldClose(window))
	{
		// HANDLE KEYPRESS
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		//RENDER OUR OBJECT
		//shader.SetColor("ucolor", Color(0, 0, 1, 1));
		shader.Use();

		for (int i = 0; i < model.Meshes().size(); i++)
		{
			model.Meshes()[i]->Draw();
		}

		//glBindVertexArray(1);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		////GLenum err;
		////while ((err = glGetError()) != GL_NO_ERROR)
		////{
		////	std::cerr << "OpenGL error: " << err << std::endl;
		////}
		//glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//UnloadMesh(ourDrawDetails);

	glfwTerminate();

	return 0;
}
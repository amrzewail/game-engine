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
#include "core/Matrix4x4.hpp"
#include "core/Quaternion.hpp"

int main(int argc, char** argv)
{
	Matrix4x4 matrix = *Matrix4x4::Identity();

	std::cout << matrix.ToString() << std::endl;

	Quaternion q1 = *new Quaternion(1, 1, 0, 0);
	Quaternion q2 = *new Quaternion(0, 1, 1, 0);

	Quaternion q3 = *(q1 * q2);

	Quaternion q4 = *Quaternion::Euler(30, 60, 85);

	matrix = *Matrix4x4::Rotate(q4);

	std::cout << matrix.ToString() << std::endl;

	std::cout << " ---- " << std::endl;

	matrix = *Matrix4x4::Translate(*new Vector(20, -10, 66));

	std::cout << matrix.ToString() << std::endl;

	std::cout << " ---- " << std::endl;

	Matrix4x4 scale = *Matrix4x4::Scale(*new Vector(2, 0.5, 6));

	std::cout << matrix.ToString() << std::endl;

	std::cout << "------------ TRS --------------- " << std::endl;

	Matrix4x4* trs = Matrix4x4::TRS(*new Vector(-10, 20, 30), *Quaternion::Euler(30, -20, 50), *new Vector(0.5, 2, 0.8));
	std::cout << trs->ToString() << std::endl;

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

	ModelAsset& model = Assets::Load<ModelAsset>("models/Jayne.fbx");


	while (!glfwWindowShouldClose(window))
	{
		// HANDLE KEYPRESS
		ProcessInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();

		for (int i = 0; i < model.Meshes().size(); i++)
		{
			model.Meshes()[i]->Draw();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
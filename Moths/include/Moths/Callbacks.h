#pragma once

#include <GLFW/glfw3.h>
#include <Moths/Core/Debug.h>

using namespace MothsEngine;


namespace MothsEngine::Internal
{

	static void glfw_error_callback(int code, const char* description)
	{
		Debug::Log(description);
		throw("glfw error");
	}

	static void glfw_window_close_callback(GLFWwindow* window)
	{
		Debug::Log("Window closed");
	}

	static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}
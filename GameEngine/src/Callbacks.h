#pragma once

static void glfw_error_callback(int code, const char* description)
{
	Log(description);
	throw("glfw error");
}

static void glfw_window_close_callback(GLFWwindow* window)
{
	Log("Window closed");
}

static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
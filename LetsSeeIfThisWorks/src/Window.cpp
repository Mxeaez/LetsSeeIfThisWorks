#include "Window.h"
#include <iostream>

Window::Window(const std::string& name, int width, int height)
{

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
}

bool Window::Running() const
{
	return !glfwWindowShouldClose(window);
}

void Window::BeginFrame() const
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::EndFrame() const
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

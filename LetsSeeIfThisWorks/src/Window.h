#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>

class Window
{
public:

	Window(const std::string& name, int width, int height);

	bool Running() const;
	void BeginFrame() const;
	void EndFrame() const;

private:

	GLFWwindow* window = nullptr;

};
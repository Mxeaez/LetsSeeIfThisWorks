#pragma once
#include <string>

#include "Window.h"
#include "World.h"

class Application
{
public:

	Application(const std::string& name, int width, int height);

	static Application& Get() { return *s_Instance; }

	void Run();

	Window& GetWindow() { return m_Window; }

private:
	static Application* s_Instance;
	Window m_Window;
	World m_World;

};
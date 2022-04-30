#pragma once

#include "glad/glad.h"
#include <iostream>
#include "Shader.h"
#include "FileSystem.h"
#include "Application.h"

int main()
{

	Application app("Learn OpenGL!", 800, 600);
	app.Run();

	return 0;
}
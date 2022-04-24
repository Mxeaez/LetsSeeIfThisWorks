#pragma once
#include <string>
#include "FileSystem.h"

class Shader
{

public:

	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	void Use() const;

private:

	unsigned int CreateShader(int shaderType, const char* shaderCode);

private:

	unsigned int id;


};
#include "Shader.h"
#include "glad/glad.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{

	const std::string& vertexCode = FileSystem::ReadFile(vertexPath);
	const std::string& fragmentCode = FileSystem::ReadFile(fragmentPath);

	const char* vertexCodeChar = vertexCode.c_str();
	const char* fragmentCodeChar = fragmentCode.c_str();

	int success;
	char infoLog[512];

	unsigned int vertex = CreateShader(GL_VERTEX_SHADER, vertexCodeChar);
	unsigned int fragment = CreateShader(GL_FRAGMENT_SHADER, fragmentCodeChar);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		std::cout << "Failure to link program: " << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() const
{
	glUseProgram(id);
}

unsigned int Shader::CreateShader(int shaderType, const char* shaderCode)
{
	int success;
	char infoLog[512];

	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error compiling shader: " << infoLog << std::endl;
	}

	return shader;
}

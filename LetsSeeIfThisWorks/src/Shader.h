#pragma once
#include <string>
#include <glm.hpp>

class Shader
{

public:

	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	void Use() const;

	void UniformMat4(const std::string& name, glm::mat4 matrix) const;
	void UniformVec3(const std::string& name, const glm::vec3& value) const;
	void UniformVec3(const std::string& name, float x, float y, float z) const;
	void UniformFloat(const std::string& name, float value) const;
	void UniformInt(const std::string& name, int value) const;

private:

	unsigned int CreateShader(int shaderType, const char* shaderCode);

private:

	unsigned int id;


};
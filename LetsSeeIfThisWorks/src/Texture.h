#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{

public:

	Texture(const std::string& filePath);

	void Use(GLenum unit) const;
	
private:

	unsigned int id = 0;

};
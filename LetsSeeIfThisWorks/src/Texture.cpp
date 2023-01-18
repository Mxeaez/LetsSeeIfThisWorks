#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "stb_image.h"
#include "glad/glad.h"
#include <iostream>

Texture::Texture(const std::string& filePath)
{
	const char* filePathChar = filePath.c_str();

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePathChar, &width, &height, &nrChannels, 0);

	if(!data)
	{
		std::cout << "Unable to load texture: " << filePath << std::endl;
	}

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Texture::Use(GLenum unit) const
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

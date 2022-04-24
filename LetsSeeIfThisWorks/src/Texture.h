#pragma once
#include <string>

class Texture
{

public:

	Texture(const std::string& filePath);

	void Use() const;
	
private:

	unsigned int id = 0;

};
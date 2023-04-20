#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace FileSystem
{

	std::string ReadFile(const std::string& filePath);
	unsigned int LoadTexture(char const* path);

}
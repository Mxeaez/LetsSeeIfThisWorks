#include "FileSystem.h"

namespace FileSystem
{

	std::string ReadFile(const std::string& filePath)
	{
		std::ifstream file;
		std::stringstream fileStream;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			file.open(filePath);

			fileStream << file.rdbuf();

			file.close();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Failure opening file: " << filePath << std::endl;
		}

		return fileStream.str();
	}

}
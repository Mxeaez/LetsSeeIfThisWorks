#pragma once

#include <vector>
#include <string>

#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model
{
public:

	Model(char* path);
	
	void Draw(Shader& shader);

private:

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	unsigned int TextureFromFile(const char* path, const std::string& directory);

private:

	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
	std::vector<Texture> m_TexturesLoaded;

};
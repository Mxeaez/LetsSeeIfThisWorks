#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../Shader.h"

class Skybox : public Component
{
public:

	Skybox(GameObject& gameObject, const std::string& name = "Skybox");

	virtual void OnUpdate() override;

private:

	unsigned int LoadSkybox(std::vector<std::string> skybox);

private:
	unsigned int m_SkyboxTexture;
	Shader m_SkyboxShader;
	unsigned int m_SkyboxVAO;
};
#include "Skybox.h"
#include "glad/glad.h"
#include "stb_image.h"
#include "../Logger.h"
#include "Camera.h"

Skybox::Skybox(GameObject& gameObject, const std::string& name)
	: m_SkyboxShader("src/Shaders/SkyboxVertexShader.vert", "src/Shaders/SkyboxFragmentShader.frag")
	, Component(gameObject, name)
{

	float skyboxVertices[] = {        
		-1000.0f,  1000.0f, -1000.0f,
		-1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,
		 1000.0f,  1000.0f, -1000.0f,
		-1000.0f,  1000.0f, -1000.0f,

		-1000.0f, -1000.0f,  1000.0f,
		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f,  1000.0f, -1000.0f,
		-1000.0f,  1000.0f, -1000.0f,
		-1000.0f,  1000.0f,  1000.0f,
		-1000.0f, -1000.0f,  1000.0f,

		 1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,

		-1000.0f, -1000.0f,  1000.0f,
		-1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f, -1000.0f,  1000.0f,
		-1000.0f, -1000.0f,  1000.0f,

		-1000.0f,  1000.0f, -1000.0f,
		 1000.0f,  1000.0f, -1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		-1000.0f,  1000.0f,  1000.0f,
		-1000.0f,  1000.0f, -1000.0f,

		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f,  1000.0f,
		 1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f,  1000.0f,
		 1000.0f, -1000.0f,  1000.0f
	};

	std::vector<std::string> faces
	{
		"src/Resources/skybox/right.jpg",
		"src/Resources/skybox/left.jpg",
		"src/Resources/skybox/top.jpg",
		"src/Resources/skybox/bottom.jpg",
		"src/Resources/skybox/front.jpg",
		"src/Resources/skybox/back.jpg"
	};
	m_SkyboxTexture = LoadSkybox(faces);

	unsigned int skyboxVBO;
	glGenVertexArrays(1, &m_SkyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(m_SkyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	m_SkyboxShader.Use();
	m_SkyboxShader.UniformInt("skybox", 0);
}

void Skybox::OnUpdate()
{
	glDepthMask(GL_FALSE);
	m_SkyboxShader.Use();
	m_SkyboxShader.UniformMat4("view", m_GameObject.GetComponent<Camera>()->GetViewMat());
	m_SkyboxShader.UniformMat4("projection", m_GameObject.GetComponent<Camera>()->GetProjMat());
	glBindVertexArray(m_SkyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

unsigned int Skybox::LoadSkybox(std::vector<std::string> skybox)
{
	unsigned int cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < skybox.size(); i++)
	{
		unsigned char* data = stbi_load(skybox[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			Logger::GetLogger()->error("Failed to load skybox");
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return cubemap;
}
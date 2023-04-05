#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "Application.h"
#include "glad/glad.h"
#include <iostream>
#include "Shader.h"
#include "FileSystem.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Application.h"
#include "Logger.h"
#include "imgui.h"
#include "imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/CameraController.h"
#include "Model.h"
#include "stb_image.h"

#include <assert.h>
#include <map>

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name, int width, int height)
	: m_Window(name, width, height)
	, m_World("World")
{

	assert(!s_Instance);
	s_Instance = this;

}

void Application::Run()
{

	Logger::Init();
	Logger::GetLogger()->trace("Game Engine starting up!");

	auto io = ImGui::GetIO();

	Shader shader("src/Shaders/VertexShader.vert", "src/Shaders/FragmentShader.frag");
	Shader outlineShader("src/Shaders/VertexShader.vert", "src/Shaders/OutlineShader.frag");
	Shader grassShader("src/Shaders/GrassVertexShader.vert", "src/Shaders/GrassFragmentShader.frag");

	GameObject* mainCamera = new GameObject("Main Camera", m_World);
    mainCamera->AddComponent<Transform>();
	mainCamera->AddComponent<Camera>();
	mainCamera->AddComponent<CameraController>();

	m_World.AddGameObject(mainCamera);

	std::vector<GameObject*> pointLights;

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	shader.Use();
	shader.UniformInt("material.diffuse", 0);
	shader.UniformInt("material.specular", 1);

	//direction light
	shader.UniformVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader.UniformVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shader.UniformVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader.UniformVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	//Point lights
	for (int i = 0; i < 4; i++)
	{
		GameObject* light = new GameObject("Light", m_World);
		light->AddComponent<Transform>();
		light->GetComponent<Transform>()->SetPosition(pointLightPositions[i]);
		auto currentLight = "pointLights[" + std::to_string(i) + "]";
		shader.UniformVec3(currentLight + ".position", pointLightPositions[0]);
		shader.UniformVec3(currentLight + ".ambient", 0.05f, 0.05f, 0.05f);
		shader.UniformVec3(currentLight + ".diffuse", 0.1f, 0.1f, 0.1f);
		shader.UniformVec3(currentLight + ".specular", 1.0f, 1.0f, 1.0f);
		shader.UniformFloat(currentLight + ".constant", 1.0f);
		shader.UniformFloat(currentLight + ".linear", 0.09f);
		shader.UniformFloat(currentLight + ".quadratic", 0.032f);

		m_World.AddGameObject(light);
		pointLights.push_back(light);
	}
	Model backPack("src/resources/backpack/backpack.obj");

	std::vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));


	float vegetationVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};


	unsigned int grassVAO;
	unsigned int grassVBO;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);

	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vegetationVertices), vegetationVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
	glBindVertexArray(0);


	std::string fileName = std::string("src/Resources/blending_transparent_window.png");

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}


	while (m_Window.Running())
	{
		glfwPollEvents();
		m_Window.BeginFrame();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_World.OnUpdate();

		auto projMat = mainCamera->GetComponent<Camera>()->GetProjMat();
		auto viewMat = mainCamera->GetComponent<Camera>()->GetViewMat();

		shader.Use();

		//Spotlight
		shader.UniformVec3("spotLight.position", mainCamera->GetComponent<Transform>()->GetPosition());
		shader.UniformVec3("spotLight.direction", mainCamera->GetComponent<Transform>()->GetForwardVector());
		shader.UniformFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.UniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

		shader.UniformVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
		shader.UniformVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
		shader.UniformVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shader.UniformFloat("spotLight.constant", 1.0f);
		shader.UniformFloat("spotLight.linear", 0.09f);
		shader.UniformFloat("spotLight.quadratic", 0.032f);
		shader.UniformVec3("viewPos", mainCamera->GetComponent<Transform>()->GetPosition());
		shader.UniformFloat("material.shininess", 32.0f);

		shader.UniformMat4("view", viewMat);
		shader.UniformMat4("projection", projMat);

		//stencil test

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		shader.UniformMat4("model", model);
		backPack.Draw(shader);

		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		outlineShader.Use();

		model = glm::scale(model, glm::vec3(1.02f, 1.02f, 1.02f));
		outlineShader.UniformMat4("model", model);
		outlineShader.UniformMat4("view", viewMat);
		outlineShader.UniformMat4("projection", projMat);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		backPack.Draw(outlineShader);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		grassShader.Use();
		glBindVertexArray(grassVAO);
		glBindTexture(GL_TEXTURE_2D, textureID);
		grassShader.UniformMat4("view", viewMat);
		grassShader.UniformMat4("projection", projMat);
		grassShader.UniformInt("texture1", 0);

		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			float distance = glm::length(mainCamera->GetComponent<Transform>()->GetPosition() - vegetation[i]);
			sorted[distance] = vegetation[i];
		}

		for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			grassShader.UniformMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		bool show_demo_window = true;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		m_Window.EndFrame();

	}

	glfwTerminate();

}

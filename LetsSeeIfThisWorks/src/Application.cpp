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
#include "Components/Skybox.h"

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

	GameObject* mainCamera = new GameObject("Main Camera", m_World);
    mainCamera->AddComponent<Transform>();
	mainCamera->AddComponent<Camera>();
	mainCamera->AddComponent<CameraController>();
	mainCamera->AddComponent<Skybox>();

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

	float planeVertices[] = {
		// positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int floorTexture = FileSystem::LoadTexture("src/Resources/floor.png");

	shader.Use();

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

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		shader.UniformMat4("model", model);
		backPack.Draw(shader);

		//floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));	// it's a bit too big for our scene, so scale it down
		shader.UniformMat4("model", model);
		glBindVertexArray(planeVAO);
		shader.UniformInt("material.texture_diffuse1", 0);
		shader.UniformInt("material.texture_specular1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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

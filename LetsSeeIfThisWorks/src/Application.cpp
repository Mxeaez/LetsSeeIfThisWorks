#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "Application.h"
#include "glad/glad.h"
#include <iostream>
#include "Shader.h"
#include "FileSystem.h"
#include "Texture.h"
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

#include <assert.h>

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

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	Shader shader("src/Shaders/VertexShader.vert", "src/Shaders/FragmentShader.frag");
	Shader lightShader("src/Shaders/LightVertexShader.vert", "src/Shaders/LightFragmentShader.frag");

	unsigned int vao;
	glGenVertexArrays(1, &vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	Texture diffuse("src/resources/pepe.png");
	Texture specular("src/resources/pepe_specular_map.png");
	diffuse.Use(GL_TEXTURE0);
	specular.Use(GL_TEXTURE1);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	Logger::Init();
	Logger::GetLogger()->trace("Game Engine starting up!");

	auto io = ImGui::GetIO();

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

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

		glBindVertexArray(lightVAO);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 lightModel = glm::mat4(1.0f);
			lightModel = glm::translate(lightModel, pointLights[i]->GetComponent<Transform>()->GetPosition());
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));

			lightShader.Use();
			lightShader.UniformMat4("model", lightModel);
			lightShader.UniformMat4("view", viewMat);
			lightShader.UniformMat4("projection", projMat);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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

		glBindVertexArray(vao);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.UniformMat4("model", model);
			shader.UniformMat4("view", viewMat); 
			shader.UniformMat4("projection", projMat);
			shader.UniformVec3("viewPos", mainCamera->GetComponent<Transform>()->GetPosition());
			shader.UniformFloat("material.shininess", 32.0f);

			glDrawArrays(GL_TRIANGLES, 0, 36);
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

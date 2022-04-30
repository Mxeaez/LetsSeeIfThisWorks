#include "CameraController.h"
#include "../Input.h"
#include "Transform.h"
#include "GLFW/glfw3.h"
#include "../Logger.h"
#include <iostream>

CameraController::CameraController(GameObject& gameObject, const std::string& name)
	: m_Speed(0.05f)
	, m_Sensitivity(0.1f)
	, Component(gameObject, name)
{
}

CameraController::CameraController(float speed, float sensitivity, GameObject& gameObject, const std::string& name)
	: m_Speed(speed)
	, m_Sensitivity(sensitivity)
	, Component(gameObject, name)
{
}

void CameraController::OnUpdate()
{

	Transform* transform = m_GameObject.GetComponent<Transform>();
	
	if (transform != nullptr)
	{

		glm::vec3 pos = transform->GetPosition();
		glm::vec3 rot = transform->GetRotation();

		if (m_FirstMouse)
		{
			m_LastX = Input::GetMouseX();
			m_LastY = Input::GetMouseY();
			m_FirstMouse = false;
		}

		float xOffset = Input::GetMouseX() - m_LastX;
		float yOffset = m_LastY - Input::GetMouseY(); // reversed since y-coordinates range from bottom to top
		m_LastX = Input::GetMouseX();
		m_LastY = Input::GetMouseY();

		xOffset *= m_Sensitivity;
		yOffset *= m_Sensitivity;

		float yaw = rot.z + xOffset;
		float pitch = rot.x + yOffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		transform->SetRotation(glm::vec3(pitch, 0.0f, yaw));

		HandleInput();

	}

}

void CameraController::HandleInput()
{
	Transform* transform = m_GameObject.GetComponent<Transform>();

	if (transform != nullptr)
	{
		glm::vec3 pos = transform->GetPosition();

		glm::vec3 newPos = pos;

		glm::vec3 forward = transform->GetForwardVector();
		glm::vec3 right = transform->GetRightVector();
		//std::cout << transform->GetForwardVector().x << "  :  " << transform->GetForwardVector().y << "  :  " << transform->GetForwardVector().z << std::endl;
		//std::cout << transform->GetRightVector().x << "  :  " << transform->GetRightVector().y << "  :  " << transform->GetRightVector().z << std::endl;
		if (Input::IsKeyPressed(GLFW_KEY_W))
			newPos = pos + (m_Speed * forward);
		if (Input::IsKeyPressed(GLFW_KEY_S))
			newPos = pos - (m_Speed * forward);
		if (Input::IsKeyPressed(GLFW_KEY_A))
			newPos = pos - (right * m_Speed);
		if (Input::IsKeyPressed(GLFW_KEY_D))
			newPos = pos + (right * m_Speed);

		transform->SetPosition(newPos);

	}
}

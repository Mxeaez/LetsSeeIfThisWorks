#include "Camera.h"
#include "gtc/matrix_transform.hpp"
#include "Transform.h"
#include <iostream>
#include "glad/glad.h"
#include "gtc/type_ptr.hpp"

Camera::Camera(GameObject& gameObject, const std::string& name)
	: m_FOV(glm::radians(45.0f))
	, m_Aspect(800.0f / 600.0f)
	, m_NearClip(0.1f)
	, m_FarClip(100.0f)
	, Component(gameObject, name)
{
	m_ProjectionMat = glm::perspective(m_FOV, m_Aspect, m_NearClip, m_FarClip);
	UpdateViewMat();

	GLint program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	unsigned int location = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_ProjectionMat));
}

Camera::Camera(float fov, float aspect, float nearClip, float farClip, GameObject& gameObject, const std::string& name)
	: m_FOV(fov)
	, m_Aspect(aspect)
	, m_NearClip(nearClip)
	, m_FarClip(farClip)
	, Component(gameObject, name)
{

	m_ProjectionMat = glm::perspective(glm::radians(m_FOV), m_Aspect, m_NearClip, m_FarClip);
	UpdateViewMat();

	GLint program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	unsigned int location = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_ProjectionMat));

}

void Camera::OnStart()
{
	UpdateViewMat();
}

void Camera::OnUpdate()
{
	UpdateViewMat();

	GLint program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	glUseProgram(program);
	unsigned int location = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_ViewMat));
}

void Camera::UpdateViewMat()
{
	Transform* transform = m_GameObject.GetComponent<Transform>();

	if (transform != nullptr)
	{
		glm::vec3 pos = transform->GetPosition();

		m_ViewMat = glm::lookAt(pos, pos + transform->GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

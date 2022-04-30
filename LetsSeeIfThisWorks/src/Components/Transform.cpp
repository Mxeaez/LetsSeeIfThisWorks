#include "Transform.h"
#include <iostream>

Transform::Transform(GameObject& gameObject, const std::string& name)
	: Component(gameObject, name)
	, m_Position(0.0f, 0.0f, 3.0f)
	, m_Rotation(0.0f, 0.0f, -90.0f)
	, m_Scale(1.0f)
{
}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, GameObject& gameObject, const std::string& name)
	: Component(gameObject, name)
	, m_Position(position)
	, m_Rotation(rotation)
	, m_Scale(scale)
{
}

glm::vec3 Transform::GetForwardVector()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Rotation.z)) * cos(glm::radians(m_Rotation.x));
	direction.y = sin(glm::radians(m_Rotation.x));
	direction.z = sin(glm::radians(m_Rotation.z)) * cos(glm::radians(m_Rotation.x));

	return glm::normalize(direction);
}

glm::vec3 Transform::GetUpVector()
{
	return glm::normalize(glm::cross(GetForwardVector(), GetRightVector()));
}

glm::vec3 Transform::GetRightVector()
{
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	auto test = glm::normalize(glm::cross(GetForwardVector(), up));

	return test;
}

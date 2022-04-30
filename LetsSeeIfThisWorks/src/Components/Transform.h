#pragma once

#include "Component.h"
#include "glm.hpp"

class Transform : public Component
{
public:

	Transform(GameObject& gameObject, const std::string& name = "Transform");
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, GameObject& gameObject, const std::string& name = "Transform");
	virtual ~Transform() {}
	
	const glm::vec3& GetPosition() const { return m_Position; }
	const glm::vec3& GetRotation() const { return m_Rotation; }
	const glm::vec3& GetScale() const { return m_Scale; }
	glm::vec3 GetForwardVector();
	glm::vec3 GetUpVector();
	glm::vec3 GetRightVector();

	void SetPosition(glm::vec3 position) { m_Position = position; }
	void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
	void SetScale(glm::vec3 scale) { m_Scale = scale; }


private:

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

};
#pragma once
#include "Component.h"
#include "glm.hpp"

class Camera : public Component
{
public:

	Camera(GameObject& gameObject, const std::string& name = "Camera");
	Camera(float fov, float aspect, float nearClip, float farClip, GameObject& gameObject, const std::string& name = "Camera");

	void OnStart() override;
	void OnUpdate() override;

	glm::mat4 GetViewMat() const;

private:

	void UpdateViewMat();

private:
	glm::mat4 m_ViewMat;
	glm::mat4 m_ProjectionMat;
	float m_FOV;
	float m_Aspect;
	float m_NearClip;
	float m_FarClip;

};
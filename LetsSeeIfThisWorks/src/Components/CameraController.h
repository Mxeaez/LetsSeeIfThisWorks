#pragma once
#include "Component.h"

class CameraController : public Component
{
public:

	CameraController(GameObject& gameObject, const std::string& name = "CameraController");
	CameraController(float speed, float sensitivity, GameObject& gameObject, const std::string& name = "CameraController");

	virtual void OnUpdate() override;

	float GetSpeed() const { return m_Speed; }
	float GetSensitivity() const { return m_Sensitivity; }

	void SetSpeed(float speed) { m_Speed = speed; }
	void SetSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }

private:

	void HandleInput();

private:

	float m_Speed;
	float m_Sensitivity;
	float m_FirstMouse = false;
	float m_LastX = 0;
	float m_LastY = 0;

};
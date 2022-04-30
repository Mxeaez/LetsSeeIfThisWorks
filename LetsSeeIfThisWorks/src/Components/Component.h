#pragma once
#include <string>

#include "../GameObject.h"

class Component
{
public:

	virtual ~Component() {}

	virtual void OnStart() {}
	virtual void OnUpdate() {}

	virtual const std::string& GetName() const { return m_Name; }
	virtual const bool IsActive() const { return m_IsActive; }
	virtual GameObject& GetGameObject() { return m_GameObject; }

protected:

	Component(GameObject& gameObject, const std::string& name)
		: m_Name(name)
		, m_GameObject(gameObject)
		, m_IsActive(true)
	{}

protected:
	std::string m_Name;
	bool m_IsActive;
	GameObject& m_GameObject;

};
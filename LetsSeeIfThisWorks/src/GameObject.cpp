#include "GameObject.h"
#include "Components/Component.h"

GameObject::GameObject(const std::string& name, World& world)
	: m_Name(name)
	, m_World(world)
	, m_IsActive(true)
{
}

void GameObject::OnStart()
{
	if (m_IsActive)
	{
		for (Component* component : m_Components)
		{
			component->OnStart();
		}
	}
}

void GameObject::OnUpdate()
{
	if (m_IsActive)
	{
		for (Component* component : m_Components)
		{
			component->OnUpdate();
		}
	}
}

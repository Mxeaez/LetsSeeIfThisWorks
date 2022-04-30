#pragma once
#include <string>
#include <vector>
#include "World.h"

class Component;

class GameObject
{
public:

	GameObject(const std::string& name, World& world);
	~GameObject() {}

	const std::string& GetName() const { return m_Name; }

	void OnStart();
	void OnUpdate();

	World& GetWorld() { return m_World; }

	template<typename T>
	T* AddComponent();
	template<typename T>
	void Destroy(T* component);

	template<typename T>
	T* GetComponent();

private:
	
	std::string m_Name;
	bool m_IsActive;
	std::vector<Component*> m_Components;
	World& m_World;

};

template<typename T>
inline T* GameObject::AddComponent()
{
	T* component = new T(*this);
	m_Components.push_back(component);
	return component;
}

template<typename T>
inline void GameObject::Destroy(T* component)
{
}

template<typename T>
inline T* GameObject::GetComponent()
{

	auto found = std::find_if(m_Components.begin(), m_Components.end(), [](Component* p) { return dynamic_cast<T*>(p); });

	if (*found)
	{
		return reinterpret_cast<T*>(*found);
	}
	else
	{
		return nullptr;
	}

}

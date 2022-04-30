#pragma once
#include <string>
#include <vector>

class GameObject;

class World
{
public:

	World(const std::string& name);

	void OnStart();
	void OnUpdate();

	void AddGameObject(GameObject* gameObject) { m_GameObjects.push_back(gameObject); }

private:
	std::string m_Name;
	std::vector<GameObject*> m_GameObjects;

};
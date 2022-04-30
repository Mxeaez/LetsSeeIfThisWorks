#include "World.h"
#include "GameObject.h"

World::World(const std::string& name)
	: m_Name(name)
{
}

void World::OnStart()
{
	for (GameObject* go : m_GameObjects)
	{
		go->OnStart();
	}
}

void World::OnUpdate()
{
	for (GameObject* go : m_GameObjects)
	{
		go->OnUpdate();
	}
}

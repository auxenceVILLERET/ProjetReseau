#include "pch.h"
#ifndef GAMEMANAGER_CPP_DEFINE
#define GAMEMANAGER_CPP_DEFINE

#include "GameManager.h"
#include "Entity.h"

GameManager* GameManager::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new GameManager();
	}
	return s_pInstance;
}

GameManager::GameManager() : m_deltaTime(0.0f)
{
}

std::vector<Entity*>& GameManager::GetEntities()
{
	return m_entities;
}

Entity* GameManager::GetEntity(uint32_t id)
{
	for (Entity* e : m_entities)
	{
		if (e->GetID() == id)
			return e;
	}
	return nullptr;
}

GameManager::~GameManager()
{

}

void GameManager::Start()
{

}

void GameManager::Update()
{
	m_deltaTime = cpuTime.delta;
	for (auto it = m_entities.begin(); it != m_entities.end(); )
	{
		Entity* entity = *it;
		entity->Update(m_deltaTime);
		++it;
	}
	for (auto it = m_entities.begin(); it != m_entities.end(); )
	{

		Entity* entity = *it;
		if (entity->m_toDestroy)
		{
			delete entity;
			it = m_entities.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameManager::Exit()
{
	while (!m_entities.empty())
	{
		delete m_entities.back();
		m_entities.pop_back();
	}
}

void GameManager::Render(int pass)
{

}


#endif // !GAMEMANAGER_CPP_DEFINE



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
	for (m_entities; !m_entities.empty(); )
	{
		Entity* entity = m_entities.back();
		m_entities.pop_back();
		delete entity;
	}
	GameManager::s_pInstance = nullptr;
}

void GameManager::Render(int pass)
{

}


#endif // !GAMEMANAGER_CPP_DEFINE



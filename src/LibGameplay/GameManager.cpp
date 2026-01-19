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



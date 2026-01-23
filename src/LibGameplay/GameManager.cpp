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

void GameManager::Update(float dt)
{
	m_deltaTime = dt;
	CheckCollisions();
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
			entity = nullptr;
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

void GameManager::UpdateRenderElements(float dt)
{
	for (auto it = m_entities.begin(); it != m_entities.end(); )
	{
		Entity* entity = *it;
		entity->UpdateRenderElements(dt);
		++it;
	}
}

void GameManager::Render(int pass)
{
	for (auto it = m_entities.begin(); it != m_entities.end(); )
	{
		Entity* entity = *it;
		entity->Render();
		++it;
	}
}

void GameManager::CheckCollisions()
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		Entity* a = m_entities[i];
		a->UpdateCollider();
		for (int j = i + 1; j < m_entities.size(); ++j)
		{
			Entity* b = m_entities[j];
			b->UpdateCollider();
			if (SphereCollision(a->m_collider, b->m_collider))
			{
				a->OnCollision(b);
				b->OnCollision(a);
			}
		}
	}
}

bool GameManager::SphereCollision(const SphereCollider& a, const SphereCollider& b) const
{
	float dx = a.center.x - b.center.x;
	float dy = a.center.y - b.center.y;
	float dz = a.center.z - b.center.z;
	float distanceSq = dx * dx + dy * dy + dz * dz;
	float radiusSum = a.radius + b.radius;
	return distanceSq <= radiusSum * radiusSum;
}




#endif // !GAMEMANAGER_CPP_DEFINE



#include "pch.h"
#ifndef ENTITY_CPP_DEFINED
#define ENTITY_CPP_DEFINED

#include "Entity.h"

uint32_t Entity::ID_COUNT = 0;

Entity::Entity()
{
	m_id = ID_COUNT++;
	m_pCpuEntity = cpuEngine.CreateEntity();
}

Entity::~Entity()
{
	cpuEngine.Release(m_pCpuEntity);
}

void Entity::Update(float dt)
{
	// YOUR CODE HERE
}

void Entity::Destroy()
{
	m_toDestroy = true;
}

#endif

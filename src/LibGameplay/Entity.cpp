#include "pch.h"
#ifndef ENTITY_CPP_DEFINED
#define ENTITY_CPP_DEFINED

#include "Entity.h"

uint32_t Entity::ID_COUNT = 0;

Entity::Entity(bool isServerSize)
{
	m_id = ID_COUNT++;

	m_isServerSide = isServerSize;
	if (!m_isServerSide)
	{
		m_pCpuEntity = cpuEngine.CreateEntity();
	}
}

Entity::~Entity()
{
	cpuEngine.Release(m_pCpuEntity);
}

void Entity::Update(float dt)
{
	// YOUR CODE HERE
}

void Entity::Render()
{

}

void Entity::UpdateCollider()
{
	XMFLOAT3 pos = m_pCpuEntity->transform.pos;
	m_collider.center = pos;
}

void Entity::Destroy()
{
	m_toDestroy = true;
}

cpu_transform& Entity::GetTransform()
{
	if (m_isServerSide)
		return m_cpuTransform;
	else
		return m_pCpuEntity->transform;
}

#endif

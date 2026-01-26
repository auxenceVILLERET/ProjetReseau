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
	if (m_isServerSide == false)
	{
		cpuEngine.Release(m_pCpuEntity);
	}
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
	XMFLOAT3 pos = GetTransform().pos;
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

void Entity::Move(float x, float y, float z)
{
	GetTransform().pos.x += x;
	GetTransform().pos.y += y;
	GetTransform().pos.z += z;
	SetDirtyFlag(DIRTY_TYPES::POS);
}

void Entity::Rotate(float x, float y, float z)
{
	GetTransform().AddYPR(x, y, z);
	SetDirtyFlag(DIRTY_TYPES::ROTATION);
}

void Entity::Scale(float scale)
{
	GetTransform().Scale(scale);
	SetDirtyFlag(DIRTY_TYPES::SCALE);
}

void Entity::SetPos(float x, float y, float z)
{
	GetTransform().pos.x = x;
	GetTransform().pos.y = y;
	GetTransform().pos.z = z;
	SetDirtyFlag(DIRTY_TYPES::POS);
}

void Entity::SetRotation(XMFLOAT4X4& rot)
{
	GetTransform().rot = rot;
	
	GetTransform().SetRotationFromMatrix();
	SetDirtyFlag(DIRTY_TYPES::ROTATION);
}

void Entity::SetScale(float scale)
{
	GetTransform().SetScaling(scale);
	SetDirtyFlag(DIRTY_TYPES::SCALE);
}

float Entity::GetScale()
{
	return (GetTransform().sca.x + GetTransform().sca.y + GetTransform().sca.z) / 3.0f;
}

void Entity::SetHealth(float health)
{
	m_health = health;
	if (m_health <= 0.0f)
		m_isAlive = false;
	SetDirtyFlag(DIRTY_TYPES::HEALTH);
}

void Entity::TakeDamage(float damage)
{
	m_health -= damage;
	if(m_health <= 0.0f)
		m_health = 0.0f;
	if(m_health <= 0.0f)
		m_isAlive = false;
	SetDirtyFlag(DIRTY_TYPES::HEALTH);
}

void Entity::Heal(float amount)
{
	if (amount == 0)
		m_health = m_maxHealth;
	m_health += amount;
	if (m_health > 100.0f)
		m_health = 100.0f;
	m_isAlive = true;
	SetDirtyFlag(DIRTY_TYPES::HEALTH);
}

#endif

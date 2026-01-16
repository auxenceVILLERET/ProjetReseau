#include "pch.h"
#include "Entity.h"

uint32_t Entity::ID_COUNT = 0;

Entity::Entity()
{
	m_id = ID_COUNT++;
}

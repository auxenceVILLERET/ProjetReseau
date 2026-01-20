#ifndef GAMEMANAGER_INL_DEFINED
#define GAMEMANAGER_INL_DEFINED

#include "GameManager.h"

template<typename T>
T* GameManager::CreateEntity(bool isServerSide)
{
	T* newEntity = new T(isServerSide);
	Entity* castedEntity = dynamic_cast<Entity*>(newEntity);
	
	if (castedEntity == nullptr)
	{
		delete newEntity;
		return nullptr;
	}
	
	m_entities.push_back(castedEntity);
	return newEntity;
}

#endif
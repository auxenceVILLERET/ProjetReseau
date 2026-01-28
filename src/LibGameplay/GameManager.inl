#ifndef GAMEMANAGER_INL_DEFINED
#define GAMEMANAGER_INL_DEFINED

#include "Entity.h"
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

	if ( isServerSide == false )
		castedEntity->InitRenderElements();
	
	m_entities.push_back(castedEntity);
	return newEntity;
}

template<typename T>
inline T* GameManager::GetEntity()
{
	for (Entity* entity : m_entities)
	{
		T* castedEntity = dynamic_cast<T*>(entity);
		if (castedEntity != nullptr)
			return castedEntity;
	}
	return nullptr;
}

#endif
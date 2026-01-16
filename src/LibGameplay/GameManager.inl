
#include "GameManager.h"

template<typename T>
T* GameManager::CreateEntity()
{
	T* newEntity = new T();
	Entity* castedEntity = dynamic_cast<Entity*>(newEntity);
	if (castedEntity == nullptr) return nullptr;
	m_entities.push_back(castedEntity);
	return newEntity;
}
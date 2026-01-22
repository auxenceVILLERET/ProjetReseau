#ifndef MANAGERMETHODS_CPP_DEFINED
#define MANAGERMETHODS_CPP_DEFINED

#include "ManagerMethods.h"

#include "Entity.h"
#include "GameManager.h"
#include "Packets.hpp"

void ManagerMethods::Init()
{
    
}

std::vector<Packet*>& ManagerMethods::GetCreationPackets()
{
    std::vector<Packet*> creationPackets;

    std::vector<Entity*> entities = GameManager::GetInstance()->GetEntities();
    for (Entity* entity : entities)
    {
        CreateEntity* p = new CreateEntity(entity->GetID(), entity->GetType(),
            entity->GetTransform().pos.x, entity->GetTransform().pos.y, entity->GetTransform().pos.z);
        creationPackets.push_back(p);
    }

    return creationPackets;
}


#endif

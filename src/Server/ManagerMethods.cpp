#ifndef MANAGERMETHODS_CPP_DEFINED
#define MANAGERMETHODS_CPP_DEFINED

#include "ManagerMethods.h"

#include "Entity.h"
#include "GameManager.h"
#include "Packets.hpp"
#include "Server.h"

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

void ManagerMethods::HandleDirtyEntities()
{
    for (Entity* entity : GameManager::GetInstance()->GetEntities())
    {
        int dirty = entity->GetDirtyFlags();
        cpu_transform t = entity->GetTransform();
        uint32_t id = entity->GetID();
        
        if (dirty && DIRTY_TYPES::POS)
        {
            SetEntityPos* packet = new SetEntityPos(id, t.pos.x, t.pos.y, t.pos.z);
            Server::GetInstance()->SendPacket(packet);
        }
        if (dirty && DIRTY_TYPES::ROTATION)
        {
            SetEntityRot* packet = new SetEntityRot(id, t.quat.x, t.quat.y, t.quat.z, t.quat.w);
            Server::GetInstance()->SendPacket(packet);
        }
        if (dirty && DIRTY_TYPES::SCALE)
        {
            float scale = t.sca.x * t.sca.y * t.sca.z / 3.0f;
            SetEntityScale* packet = new SetEntityScale(id, scale);
            Server::GetInstance()->SendPacket(packet);
        }

        entity->ClearDirtyFlags();
    }
}


#endif

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

void ManagerMethods::SendCreationPackets(ClientInfo* pTarget)
{
    for (Entity* entity : GameManager::GetInstance()->GetEntities())
    {
        EntityType type = entity->GetType();
        cpu_transform t = entity->GetTransform();
        XMFLOAT3 pos = t.pos;
        float scale = t.sca.x + t.sca.y + t.sca.z / 3.0f;

        CreateEntity* packet = new CreateEntity(entity->GetID(), type, pos.x, pos.y, pos.z, scale);
        Server::GetInstance()->SendTargetedPacket(packet, pTarget);
    }
}

void ManagerMethods::HandleDirtyEntities()
{
    for (Entity* entity : GameManager::GetInstance()->GetEntities())
    {
        int dirty = entity->GetDirtyFlags();
        cpu_transform t = entity->GetTransform();
        uint32_t id = entity->GetID();
        
        if (dirty & DIRTY_TYPES::POS)
        {
            SetEntityPos* packet = new SetEntityPos(id, t.pos.x, t.pos.y, t.pos.z);
            Server::GetInstance()->SendPacket(packet);
        }
        if (dirty & DIRTY_TYPES::ROTATION)
        {
            SetEntityRot* packet = new SetEntityRot(id, entity->GetTransform().rot);
            Server::GetInstance()->SendPacket(packet);
        }
        if (dirty & DIRTY_TYPES::SCALE)
        {
            float scale = t.sca.x * t.sca.y * t.sca.z / 3.0f;
            SetEntityScale* packet = new SetEntityScale(id, scale);
            Server::GetInstance()->SendPacket(packet);
        }

        entity->ClearDirtyFlags();
    }
}


#endif

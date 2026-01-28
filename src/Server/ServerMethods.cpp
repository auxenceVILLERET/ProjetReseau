#ifndef MANAGERMETHODS_CPP_DEFINED
#define MANAGERMETHODS_CPP_DEFINED

#include "ServerMethods.h"

#include "Entity.h"
#include "GameManager.h"
#include "Packets/Packets.h"
#include "Server.h"
#include "Gameplay.h"
#include "Asteroid.h"


void ServerMethods::Init()
{
    
}

void ServerMethods::SendCreationPackets(ClientInfo* pTarget)
{
    for (Entity* entity : GameManager::GetInstance()->GetEntities())
    {
        EntityType type = entity->GetType();
        cpu_transform t = entity->GetTransform();
        XMFLOAT3 pos = t.pos;
        XMFLOAT3 dir = t.dir;
        float scale = (t.sca.x + t.sca.y + t.sca.z) / 3.0f;

        CreateEntity* packet = new CreateEntity(entity->GetID(), type, pos, dir, scale);
        Server::GetInstance()->SendTargetedPacket(packet, pTarget);
    }
}

void ServerMethods::HandleDirtyEntities()
{
    for (Entity* entity : GameManager::GetInstance()->GetEntities())
    {
        int dirty = entity->GetDirtyFlags();
        cpu_transform t = entity->GetTransform();
        uint32_t id = entity->GetID();

        if(dirty == 0)
			continue;

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
        if(dirty & DIRTY_TYPES::HEALTH)
        {
            SetEntityHealthPacket* packet = new SetEntityHealthPacket(id, entity->GetHealth());
            Server::GetInstance()->SendPacket(packet);
		}

        entity->ClearDirtyFlags();
    }
}

void ServerMethods::HandleDestroyedEntities(int& destroyed)
{
    for (Entity* entity : GameManager::GetInstance()->GetEntities())
    {
        if (entity->GetToDestroy() == false) continue;
        if(entity->GetType() == EntityType::ASTEROID)
			destroyed++;

        DestroyEntityPacket* packet = new DestroyEntityPacket(entity->GetID());
        Server::GetInstance()->SendPacket(packet);
    }
}

void ServerMethods::InitMap()
{
    for (int i = 0; i < ASTEROID_COUNT; ++i)
    {
        Asteroid* asteroid = GameManager::GetInstance()->CreateEntity<Asteroid>(true);

        XMFLOAT3 pos;
        pos.x = RandomRange(BORDER_MIN, BORDER_MAX);
        pos.y = RandomRange(BORDER_MIN, BORDER_MAX);
        pos.z = RandomRange(BORDER_MIN, BORDER_MAX);

        asteroid->SetPos(pos.x, pos.y, pos.z);
        float size = RandomRange(0.5f, 5.0f);
        asteroid->Init(size);

    }
}

void ServerMethods::RespawnAsteroid(int& asteroidDestroy)
{
    if (asteroidDestroy >= 10)
    {
        int toRespawn = RandomRange(5,15);
        for (int i = 0; i < toRespawn; i++)
        {
            Asteroid* asteroid = GameManager::GetInstance()->CreateEntity<Asteroid>(true);

            XMFLOAT3 pos;
            pos.x = RandomRange(BORDER_MIN, BORDER_MAX);
            pos.y = RandomRange(BORDER_MIN, BORDER_MAX);
            pos.z = RandomRange(BORDER_MIN, BORDER_MAX);

            asteroid->SetPos(pos.x, pos.y, pos.z);
            float size = RandomRange(0.5f, 5.0f);
			asteroid->Init(size);

            asteroid->ClearDirtyFlags();
			CreateEntity* packet = new CreateEntity(asteroid->GetID(), asteroid->GetType(), asteroid->GetTransform().pos, asteroid->GetTransform().dir, asteroid->GetScale());
			Server::GetInstance()->SendPacket(packet);
        }
		asteroidDestroy = 0;
    }
}


#endif

#include "pch.h"
#ifndef CLIENTMETHODS_CPP_DEFINED
#define CLIENTMETHODS_CPP_DEFINED
#include "ClientMethods.h"

#include "GameManager.h"
#include "Player.h"
#include "Projectile.h"

Entity* ClientMethods::CopyEntity(CreateEntity* entityPacket)
{
    EntityType type = entityPacket->type;
    Entity* pEntity = nullptr;

    Entity* search = GameManager::GetInstance()->GetEntity(entityPacket->id);
    if (search != nullptr) return search;
    
    switch (type)
    {
    case EntityType::PLAYER:
        pEntity = GameManager::GetInstance()->CreateEntity<Player>();
        break;
    case EntityType::PROJECTILE:
        pEntity = GameManager::GetInstance()->CreateEntity<Projectile>();
        break;
    default:
        break;
    }

    if (pEntity == nullptr) return pEntity;

    pEntity->GetTransform().pos = XMFLOAT3(entityPacket->x, entityPacket->y, entityPacket->z);
    pEntity->SetID(entityPacket->id);
    return pEntity;
}

#endif

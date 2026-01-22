#include "pch.h"
#ifndef CLIENTMETHODS_CPP_DEFINED
#define CLIENTMETHODS_CPP_DEFINED
#include "ClientMethods.h"

#include "Client.h"
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

bool ClientMethods::MoveEntity(uint32_t id, XMFLOAT3 position)
{
    Entity* entity = GameManager::GetInstance()->GetEntity(id);
    if (entity == nullptr) return false;

    MoveEntityPacket* packet = new MoveEntityPacket(id, position.x, position.y, position.z);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

bool ClientMethods::RotateEntity(uint32_t id, XMFLOAT3 rotation)
{
    Entity* entity = GameManager::GetInstance()->GetEntity(id);
    if (entity == nullptr) return false;

    RotateEntityPacket* packet = new RotateEntityPacket(id, rotation.x, rotation.y, rotation.z);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

bool ClientMethods::ScaleEntity(uint32_t id, float n)
{
    Entity* entity = GameManager::GetInstance()->GetEntity(id);
    if (entity == nullptr) return false;

    ScaleEntityPacket* packet = new ScaleEntityPacket(id, n);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

#endif

#include "pch.h"
#ifndef CLIENTMETHODS_CPP_DEFINED
#define CLIENTMETHODS_CPP_DEFINED
#include "ClientMethods.h"

#include "Client.h"
#include "GameManager.h"
#include "Player.h"
#include "Projectile.h"
#include "Asteroid.h"

std::vector<ChatLine> ClientMethods::s_chatMessages;

Entity* ClientMethods::CopyEntity(CreateEntity* entityPacket)
{
    EntityType type = entityPacket->type;
    Entity* pEntity = nullptr;
    
    switch (type)
    {
    case EntityType::PLAYER:
        pEntity = GameManager::GetInstance()->CreateEntity<Player>();
        break;
    case EntityType::PROJECTILE:
        pEntity = GameManager::GetInstance()->CreateEntity<Projectile>();
        break;
    case EntityType::ASTEROID:
        pEntity = GameManager::GetInstance()->CreateEntity<Asteroid>();
        break;
    default:
        break;
    }

    if (pEntity == nullptr) return pEntity;

    pEntity->GetTransform().pos = XMFLOAT3(entityPacket->x, entityPacket->y, entityPacket->z);
    pEntity->SetScale(entityPacket->scale);
    pEntity->SetID(entityPacket->id);
    if (entityPacket->dx != 0 || entityPacket->dy != 0 || entityPacket->dz != 0)
        pEntity->GetTransform().LookTo(entityPacket->dx, entityPacket->dy, entityPacket->dz);
    return pEntity;
}

bool ClientMethods::MoveEntity(uint32_t id, XMFLOAT3 position)
{
    MoveEntityPacket* packet = new MoveEntityPacket(id, position.x, position.y, position.z);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

bool ClientMethods::RotateEntity(uint32_t id, XMFLOAT3 rotation)
{
    RotateEntityPacket* packet = new RotateEntityPacket(id, rotation.x, rotation.y, rotation.z);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

bool ClientMethods::ScaleEntity(uint32_t id, float n)
{
    ScaleEntityPacket* packet = new ScaleEntityPacket(id, n);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

bool ClientMethods::ChangePlayerSpeed(uint32_t id, float delta)
{
    ChangePlayerSpeedPacket* packet = new ChangePlayerSpeedPacket(id, delta);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

bool ClientMethods::ShootProjectile(uint32_t shooterId, XMFLOAT3 pos, XMFLOAT3 dir)
{
    ShootProjectilePacket* packet = new ShootProjectilePacket(shooterId, pos.x, pos.y, pos.z, dir.x, dir.y, dir.z);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

bool ClientMethods::SetActiveState(uint32_t id, bool isActive)
{
    SetActiveStatePacket* packet = new SetActiveStatePacket(id, isActive);
    Client::GetInstance()->SendPacket(packet);
	return true;
}

bool ClientMethods::SetPosition(uint32_t id, XMFLOAT3 position)
{
    SetEntityPos* packet = new SetEntityPos(id, position.x, position.y, position.z);
    Client::GetInstance()->SendPacket(packet);
    return true;
}

bool ClientMethods::SetDirection(uint32_t id, XMFLOAT3 rotation)
{
    SetEntityDirPacket* packet = new SetEntityDirPacket(id, rotation.x, rotation.y, rotation.z);
    Client::GetInstance()->SendPacket(packet);
	return true;
}

bool ClientMethods::SendChatMessage(uint32_t id, const char* message)
{
    ChatMessagePacket* packet = new ChatMessagePacket(id, message);
    Client::GetInstance()->SendPacket(packet);
	return true;
}

bool ClientMethods::AddChatMessage(const std::string& user, const std::string& msg)
{
    ChatLine line;
    line.user = user;
    line.text = msg;
    s_chatMessages.push_back(line);

    if(s_chatMessages.size() > 10)
    {
        s_chatMessages.erase(s_chatMessages.begin());
	}
	return true;
}

bool ClientMethods::SetHealth(uint32_t id, float health)
{
    SetEntityHealthPacket* packet = new SetEntityHealthPacket(id, health);
    Client::GetInstance()->SendPacket(packet);
	return true;
}



#endif

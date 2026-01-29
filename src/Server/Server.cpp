#ifndef SERVER_CPP_DEFINED
#define SERVER_CPP_DEFINED

#include "Server.h"

#include <iostream>
#include <WS2tcpip.h>

#include "GameManager.h"
#include "ServerMethods.h"
#include "Message.h"
#include "Packets/Packets.h"
#include "Player.h"
#include "Projectile.h"
#include "../LibNetwork/Socket.h"
#include "Packets/SetPlayerUsernamePacket.hpp"

Server* Server::m_pInstance = nullptr;

Server::Server() : m_udpSocket()
{
    memset(m_buffer, 0, sizeof(m_buffer));
}

Server::~Server()
{
    for (ClientInfo* client : m_vClients)
    {
        delete client;
    }
    m_vClients.clear();
}

Server* Server::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new Server();
        m_pInstance->Init();
    }
    return m_pInstance;
}

void Server::Init()
{
    srand((unsigned int)time(NULL));
    if (!m_udpSocket.Bind("127.0.0.1", 1888))
    {
        std::cout << "Connexion attempt failed.\n" << Sockets::GetError() << "\n";
    }

    m_chrono.Start();
    CreateThread(nullptr, 0, ReceiveThread, this, 0, nullptr);
	ServerMethods::InitMap();
    
    std::cout << "Connection established.\n";
}

UDPSocket* Server::GetSocket()
{
    return &m_udpSocket;
}

bool Server::LogUser(sockaddr_in newAddr, std::string username)
{
    bool exists = false;

    ClientInfo* pExisting = nullptr;
    
    for (ClientInfo* client : m_vClients)
    {
        if (username == client->username)
        {
            exists = true;
            pExisting = client;
        }
    }

    char ip[23];
    inet_ntop(AF_INET, &newAddr.sin_addr, ip, INET_ADDRSTRLEN);
    int port = ntohs(newAddr.sin_port);
    std::string ipStr = ip;
    
    if (exists)
    {
        if (pExisting->connected == true) return false;

        if (pExisting->connected == false)
        {
            pExisting->username = username;
            pExisting->port = port;
            pExisting->ip = ipStr;
            pExisting->sockAddr = newAddr;
            
            return true;
        }
    }
    
    ClientInfo* newClient = new ClientInfo();
    newClient->username = username;
    newClient->port = port;
    newClient->ip = ipStr;
    newClient->sockAddr = newAddr;
    
    m_vClients.push_back(newClient);

    return true;
}

void Server::GlobalMsg(const char* msg)
{
    for (ClientInfo* client : m_vClients)
    {
        if (client->connected == false) continue;
        // std::cout << "Sent global message to " << client.username << "\n";
        m_udpSocket.SendTo(msg, Message::BUFFER_SIZE + 1, client->sockAddr);
    }
}

void Server::PrintMessage(const char* msg, sockaddr_in addr)
{
    char ip[23];
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
    int port = ntohs(addr.sin_port);

    std::cout << "[" << ip << " : " << port << "] : " << msg << "\n";
}

DWORD Server::ReceiveThread(LPVOID lpParam)
{
    Server* pInstance = static_cast<Server*>(lpParam);
    while (true)
    {
        char responseBuffer[1024];
        sockaddr_in target{};
        int response = pInstance->m_udpSocket.ReceiveFrom(responseBuffer, 1024, target);
        
        if (response == -1) continue;

        bool ignoreMessage = false;
        
        for (int i = 0; i < pInstance->m_vClients.size(); i++)
        {
            ClientInfo* client = pInstance->m_vClients[i];
            char ip[23];
            if (!inet_ntop(AF_INET, &target.sin_addr, ip, INET_ADDRSTRLEN))
                continue;
            
            int port = ntohs(target.sin_port);
            std::string ipStr = ip;
        
            if (ipStr != client->ip || port != client->port)
                continue;
            if (client->connected == false)
                ignoreMessage = true;
        }

        if (ignoreMessage) continue;
        
        Message* msg = new Message();
        std::vector<Packet*> packets = msg->Deserialize(responseBuffer);
        delete msg;
        
        pInstance->m_packetProtection.Enter();
        for (int i = 0; i < packets.size(); i++)
        {
            ReceivedPacket packet {packets[i], target};
            pInstance->m_packets.push_back(packet);
        }
        pInstance->m_packetProtection.Leave();
    }
    
    return 1;
}

void Server::HandlePackets()
{
    m_packetProtection.Enter();
    for (ReceivedPacket rPacket : m_packets)
    {
        Packet* packet = rPacket.packet;
        PacketType type = (PacketType)packet->GetType();

        packet->PrintInfo(false);
        
        if (type == PING_PONG)
        {
            PingPongPacket* casted = dynamic_cast<PingPongPacket*>(packet);
            if (casted == nullptr) continue;
            
            LogUser(rPacket.sockAddr, casted->username);
            
            ClientInfo* pClient = FindClient(casted->username);

            pClient->connected = !pClient->connected;

            PingPongPacket* responsePkt = new PingPongPacket(casted->username, false);
            SendTargetedPacket(responsePkt, pClient);
            
            if (pClient->connected == true)
            {
                ServerMethods::SendCreationPackets(pClient);
                Player* p = GameManager::GetInstance()->CreateEntity<Player>(true);
                p->GetTransform().pos = GetSpawnPoint();
                pClient->playerId = p->GetID();
            
                XMFLOAT3 dir;
                dir.x = 0.0f - p->GetTransform().pos.x;
                dir.y = 0.0f - p->GetTransform().pos.y;
                dir.z = 0.0f - p->GetTransform().pos.z;
                float length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
                if (length > 0.0001f)
                {
                    dir.x /= length;
                    dir.y /= length;
                    dir.z /= length;
                }
            
                p->GetTransform().LookTo(dir.x, dir.y, dir.z);
                p->SetDirtyFlag(DIRTY_TYPES::ROTATION);
                CreateEntity* createPacket = new CreateEntity(p->GetID(), p->GetType());
                SendPacket(createPacket);

                SendTargetedPacket(new SetPlayerIDPacket(p->GetID()), pClient);

                p->SetStats(pClient->killCount, pClient->deathCount, pClient->score);
                p->SetUsername(casted->username);
                pClient->playerId = p->GetID();

                SetPlayerStatsPacket* sPacket = new SetPlayerStatsPacket(p->GetID(), p->GetKillCount(), p->GetDeathCount(), p->GetScore());
                SendPacket(sPacket);
                SetPlayerUsernamePacket* uPacket = new SetPlayerUsernamePacket(p->GetID(), pClient->username);
                SendPacket(uPacket);
                
                MessageConnected(pClient);
            }
            else if (pClient->connected == false)
            {
                Entity* e = GameManager::GetInstance()->GetEntity(pClient->playerId);
                if (e != nullptr)
                    e->Destroy();
            }
        }
        else if (type == ROTATE_ENTITY)
        {
            RotateEntityPacket* casted = dynamic_cast<RotateEntityPacket*>(packet);
            if (casted == nullptr) continue;

            Entity* e = GameManager::GetInstance()->GetEntity(casted->id);
            if (e == nullptr) continue;

            e->Rotate(casted->x, casted->y, casted->z);
        }
        else if (type == CHANGE_PLAYER_SPEED)
        {
            ChangePlayerSpeedPacket* casted = dynamic_cast<ChangePlayerSpeedPacket*>(packet);
            if (casted == nullptr) continue;

            Entity* e = GameManager::GetInstance()->GetEntity(casted->id);
            Player* p = dynamic_cast<Player*>(GameManager::GetInstance()->GetEntity(casted->id));
            if (p == nullptr) continue;

            p->AddSpeed(casted->delta);

            SetPlayerSpeedPacket* nPacket = new SetPlayerSpeedPacket(p->GetID(), p->GetSpeed());
            SendPacket(nPacket);
        }
        else if (type == SHOOT_PROJECTILE)
        {
            ShootProjectilePacket* casted = dynamic_cast<ShootProjectilePacket*>(packet);
            if (casted == nullptr) continue;

            Projectile* e = GameManager::GetInstance()->CreateEntity<Projectile>(true);
            if (e == nullptr) continue;
            e->Init({casted->px, casted->py, casted->pz}, {casted->dx, casted->dy, casted->dz});
            Player* player = dynamic_cast<Player*>(GameManager::GetInstance()->GetEntity(casted->shooterId));
            if (player != nullptr)
                e->SetShooter(player);

            CreateEntity* nPacket = new CreateEntity(e->GetID(), e->GetType(), e->GetTransform().pos, e->GetTransform().dir, e->GetScale());
            SendPacket(nPacket);
        }
        else if(type == SET_ACTIVE_STATE)
        {
            SetActiveStatePacket* casted = dynamic_cast<SetActiveStatePacket*>(packet);
            if (casted == nullptr) continue;

            Entity* e = GameManager::GetInstance()->GetEntity(casted->id);

            if (e == nullptr) continue;
            
            if (casted->isActive)
                e->SetActive();
            else
                e->SetInactive();

            if (e->GetType() == PLAYER)
            {
                Player* p = dynamic_cast<Player*>(e);

                std::string killName = p->GetKillName();

                if (p->GetActiveState() == false)
                {
                    ChatMessagePacket* deathMessage = new ChatMessagePacket("ServeR", p->GetName() + " was destroyed by " + killName);
                    SendPacket(deathMessage);
                }
            }

			SetActiveStatePacket* nPacket = new SetActiveStatePacket(casted->id, casted->isActive);
			SendPacket(nPacket);
		}
        else if (type == SET_ENTITY_POS)
        {
            SetEntityPos* casted = dynamic_cast<SetEntityPos*>(packet);
            if (casted == nullptr) continue;
            Entity* e = GameManager::GetInstance()->GetEntity(casted->id);
            if (e == nullptr) continue;
            e->SetPos(casted->x, casted->y, casted->z);
        }
        else if (type == SET_ENTITY_DIR)
        {
            SetEntityDirPacket* casted = dynamic_cast<SetEntityDirPacket*>(packet);
            if (casted == nullptr) continue;

            Entity* e = GameManager::GetInstance()->GetEntity(casted->id);
            if (e == nullptr) continue;

            e->GetTransform().LookTo(casted->dx, casted->dy, casted->dz);
            e->SetDirtyFlag(DIRTY_TYPES::ROTATION);
        }
        else if (type == SET_HEALTH)
        {
            SetEntityHealthPacket* casted = dynamic_cast<SetEntityHealthPacket*>(packet);
            if (casted == nullptr) continue;

            Entity* e = GameManager::GetInstance()->GetEntity(casted->id);
            if (e == nullptr) continue;

            e->SetHealth(casted->health);
        }
        else if(type == CHAT_MESSAGE)
        {
            ChatMessagePacket* casted = dynamic_cast<ChatMessagePacket*>(packet);

            if (casted == nullptr) continue;

			ChatMessagePacket* nPacket = new ChatMessagePacket(casted->username, casted->text);
			SendPacket(nPacket);
		}
        else if (type == CHANGE_COLOR_SHIP)
        {
            ChangeColorShipPacket* casted = dynamic_cast<ChangeColorShipPacket*>(packet);
            if (casted == nullptr) continue;

            Entity* e = GameManager::GetInstance()->GetEntity(casted->id);
            Player* p = dynamic_cast<Player*>(e);

            if (p == nullptr) continue;

            p->ChangeColorShip(casted->index);

			ChangeColorShipPacket* nPacket = new ChangeColorShipPacket(casted->id, casted->index);
			SendPacket(nPacket);
        }
        else if (type == CHANGE_COLOR_PARTICLE)
        {
            ChangeColorParticlePacket* casted = dynamic_cast<ChangeColorParticlePacket*>(packet);
            if (casted == nullptr) continue;
            Entity* e = GameManager::GetInstance()->GetEntity(casted->id);
            Player* p = dynamic_cast<Player*>(e);
            if (p == nullptr) continue;
            p->ChangeColorParticle(casted->index);
            ChangeColorParticlePacket* nPacket = new ChangeColorParticlePacket(casted->id, casted->index);
            SendPacket(nPacket);
        }

    }

    for (int i = 0; i < m_packets.size(); i++)
    {
        delete m_packets[i].packet;
    }
    
    m_packets.clear();
    m_packetProtection.Leave();
}

void Server::SendPacket(Packet* packet)
{
    // std::cout << "Registered Global Packet : ";
    packet->PrintInfo(true);
    
    if (m_pendingMessages.size() == 0)
    {
        Message* msg = new Message(true);
        m_pendingMessages.push_back(msg);
    }

    Message* lastMessage = m_pendingMessages.back();
    if (lastMessage->AddPacket(packet) == false)
    {
        Message* msg = new Message(true);
        msg->AddPacket(packet);
        m_pendingMessages.push_back(msg);
    }
}

void Server::SendTargetedPacket(Packet* packet, ClientInfo* pTarget)
{
    // std::cout << "Registered packet of type " << PacketTypeNames[packet->GetType()] << " for " << pTarget->username << std::endl;
    if (pTarget == nullptr) return;
    if (pTarget->connected == false) return;

    bool hasCreated = false;
    
    if (!m_pendingTargetedMessage.contains(pTarget))
    {
        m_pendingTargetedMessage[pTarget] = std::vector<Message*>();
        hasCreated = true;
    }

    if (m_pendingTargetedMessage[pTarget].size() == 0)
    {
        Message* msg = new Message(true);
        m_pendingTargetedMessage[pTarget].push_back(msg);
    }

    Message* lastMessage = m_pendingTargetedMessage[pTarget].back();
    if (lastMessage->AddPacket(packet) == false)
    {
        Message* msg = new Message(true);
        msg->AddPacket(packet);
        m_pendingTargetedMessage[pTarget].push_back(msg);
    }
}

ClientInfo* Server::FindClient(std::string username)
{
    for (ClientInfo* client : m_vClients)
    {
        if (client->username == username)
            return client;
    }
    return nullptr;
}

void Server::ClearMessages()
{
    for (int i = 0; i < m_pendingMessages.size(); i++)
    {
        delete m_pendingMessages[i];
    }
    m_pendingMessages.clear();

    for ( auto& [key, value] : m_pendingTargetedMessage)
    {
        for (int i = 0; i < value.size(); i++)
        {
            delete value[i];
        }
        value.clear();
    }
}

void Server::Update()
{
    HandlePackets();
    GameManager::GetInstance()->Update(m_chrono.Reset());
    ServerMethods::HandleDirtyEntities();
    ServerMethods::HandleDestroyedEntities(m_asteroidDestroyedCount);
    GameManager::GetInstance()->ClearDestroyedEntities();
	ServerMethods::RespawnAsteroid(m_asteroidDestroyedCount);

    for (Message* msg : m_pendingMessages)
    {
        msg->Serialize(m_buffer);
        GlobalMsg(m_buffer);
        msg->ClearPackets();
    }

    m_pendingMessages.clear();

    for ( auto& [key, value] : m_pendingTargetedMessage)
    {
        for (Message* msg : value)
        {
            msg->Serialize(m_buffer);
            m_udpSocket.SendTo(m_buffer, Message::BUFFER_SIZE + 1, key->sockAddr);
            msg->ClearPackets();
        }
        value.clear();
    }

    ClearMessages();
}

void Server::MessageConnected(ClientInfo* pClient)
{
    std::string msg = pClient->username + " has connected.";
    ChatMessagePacket* chatPacket = new ChatMessagePacket("Server", msg);
	SendPacket(chatPacket);
}

#endif

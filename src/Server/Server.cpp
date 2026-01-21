#ifndef SERVER_CPP_DEFINED
#define SERVER_CPP_DEFINED

#include "Server.h"

#include <iostream>
#include <WS2tcpip.h>

#include "GameManager.h"
#include "Message.h"
#include "Packets.hpp"
#include "Player.h"
#include "../LibNetwork/Socket.h"

Server* Server::m_pInstance = nullptr;

Server::Server() : m_udpSocket()
{
    
}

Server::~Server()
{
    
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
    if (!m_udpSocket.Bind("127.0.0.1", 1888))
    {
        std::cout << "Connexion attempt failed.\n" << Sockets::GetError() << "\n";
    }
    
    CreateThread(nullptr, 0, ReceiveThread, this, 0, nullptr);
    
    std::cout << "Connection established.\n";
}

UDPSocket* Server::GetSocket()
{
    return &m_udpSocket;
}

bool Server::LogUser(sockaddr_in newAddr, std::string username)
{
    bool exists = false;

    for (ClientInfo client : m_vClients)
    {
        if (username == client.username)
            exists = true;
    }

    if (exists) return false;

    char ip[23];
    inet_ntop(AF_INET, &newAddr.sin_addr, ip, INET_ADDRSTRLEN);
    int port = ntohs(newAddr.sin_port);
    std::string ipStr = ip;

    ClientInfo client;
    client.username = username;
    client.port = port;
    client.ip = ipStr;
    client.sockAddr = newAddr;
    
    m_vClients.push_back(client);

    return true;
}

void Server::GlobalMsg(const char* msg)
{
    for (ClientInfo client : m_vClients)
    {
        std::cout << "Sent global message to " << client.username << "\n";
        m_udpSocket.SendTo(msg, Message::BUFFER_SIZE + 1, client.sockAddr);
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
        char responseBuffer[1024 + 1];
        sockaddr_in target;
        int response = pInstance->m_udpSocket.ReceiveFrom(responseBuffer, 1024, target);
        
        Message msg;
        std::vector<Packet*> packets = msg.Deserialize(responseBuffer);

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

        std::cout << "Received Packet Type: " << PacketTypeNames[type] << std::endl;
        
        if (type == PING_PONG)
        {
            PingPongPacket* casted = dynamic_cast<PingPongPacket*>(packet);
            if (casted == nullptr) continue;

            LogUser(rPacket.sockAddr, casted->username);
            ClientInfo* pClient = FindClient(casted->username);
            
            PingPongPacket* responsePkt = new PingPongPacket(casted->username, false);
            SendTargetedPacket(responsePkt, pClient);
            
            Player* p = GameManager::GetInstance()->CreateEntity<Player>(true);
            p->GetTransform().pos = GetSpawnPoint();
            CreateEntity* createPacket = new CreateEntity(p->GetID(), p->GetType());
            SendPacket(createPacket);

            SendTargetedPacket(new SetPlayerIDPacket(p->GetID()), pClient);
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
    std::cout << "Registered Global packet of type " << PacketTypeNames[packet->GetType()] << std::endl;
    
    if (m_pendingMessages.size() == 0)
    {
        Message msg;
        m_pendingMessages.push_back(msg);
    }

    Message* lastMessage = &m_pendingMessages.back();
    if (lastMessage->AddPacket(packet) == false)
    {
        Message msg;
        msg.AddPacket(packet);
        m_pendingMessages.push_back(msg);
    }
}

void Server::SendTargetedPacket(Packet* packet, ClientInfo* pTarget)
{
    std::cout << "Registered packet of type " << PacketTypeNames[packet->GetType()] << " for " << pTarget->username << std::endl;
    if (pTarget == nullptr) return;
    
    if (!m_pendingTargetedMessage.contains(pTarget))
    {
        m_pendingTargetedMessage[pTarget] = std::vector<Message>();
    }

    if (m_pendingMessages.size() == 0)
    {
        Message msg;
        m_pendingTargetedMessage[pTarget].push_back(msg);
    }

    Message* lastMessage = &m_pendingTargetedMessage[pTarget].back();
    if (lastMessage->AddPacket(packet) == false)
    {
        Message msg;
        msg.AddPacket(packet);
        m_pendingTargetedMessage[pTarget].push_back(msg);
    }
    
}

ClientInfo* Server::FindClient(std::string username)
{
    for (ClientInfo& client : m_vClients)
    {
        if (client.username == username)
            return &client;
    }
    return nullptr;
}

void Server::Update()
{
    HandlePackets();
    GameManager::GetInstance()->Update();

    for (Message msg : m_pendingMessages)
    {
        msg.Serialize(m_buffer);
        GlobalMsg(m_buffer);
        msg.ClearPackets();
    }
    m_pendingMessages.clear();

    for ( auto& [key, value] : m_pendingTargetedMessage)
    {
        for (Message msg : value)
        {
            msg.Serialize(m_buffer);
            m_udpSocket.SendTo(m_buffer, Message::BUFFER_SIZE + 1, key->sockAddr);
            msg.ClearPackets();
        }
        value.clear();
    }
    
}


#endif
